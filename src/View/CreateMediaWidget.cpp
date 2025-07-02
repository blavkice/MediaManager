#include "CreateMediaWidget.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QShortcut>
#include <QVBoxLayout>

#include "../Model/AddVisitor.h"

CreateMediaWidget::CreateMediaWidget(QWidget* parent, Media* media)
    : QWidget(parent), currentMedia(media), createButton(nullptr) {
    if (media == nullptr) {
        throw std::invalid_argument("media cannot be null");
    }

    auto layout = new QVBoxLayout(this);
    auto scrollArea = new QScrollArea(this);
    auto contentWidget = new QWidget(scrollArea);
    contentLayout = new QVBoxLayout(contentWidget);

    // note: clearing is not to be done here

    // to add image from dialog
    contentLayout->addWidget(new QLabel("Image Path:"));
    imagePathEdit = new QLineEdit();
    imagePathEdit->setReadOnly(true);
    imagePathEdit->setText(media->getImagePath());
    contentLayout->addWidget(imagePathEdit);
    chooseButton = new QPushButton("Choose");
    contentLayout->addWidget(chooseButton);
    connect(chooseButton, &QPushButton::clicked, this, &CreateMediaWidget::chooseImage);

    // visualize the input fields for the selected concrete media (with safe memory management)
    addVisitor = std::make_unique<AddVisitor>(contentLayout);
    media->accept(addVisitor.get());

    createButton = new QPushButton("Create", this);
    contentLayout->addWidget(createButton);

    // in fullscreen mode this is the only way to make "enter" key work...
    QShortcut const* enterShortcut = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    connect(enterShortcut, &QShortcut::activated, createButton, &QPushButton::click);

    contentWidget->setLayout(contentLayout);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    setLayout(layout);

    connect(createButton, &QPushButton::clicked, this, [this]() {
        if (!currentMedia) return;
        // has the user chosen a new image?
        QString chosenPath = imagePathEdit->text();
        // if the user has not chosen a new image, we keep the old one
        if (!chosenPath.isEmpty() && chosenPath != currentMedia->getImagePath() && QFile::exists(chosenPath)) {
            // save the image to the media directory
            QString appDirPath = QCoreApplication::applicationDirPath();
            QDir dir(appDirPath);
            QString mediaDirPath = dir.filePath("media");
            if (!dir.exists(mediaDirPath)) dir.mkpath(mediaDirPath);
            QString destFilePath = mediaDirPath + "/" + currentMedia->getId() + ".jpg";
            QImage img(chosenPath);
            if (!img.isNull()) {
                img.save(destFilePath, "JPG", 65);
                currentMedia->setImagePath(currentMedia->getId() + ".jpg");
            } else {
                // if the image is not valid, we set a default image
                currentMedia->setImagePath(":default.jpg");
            }
        }
        // save the input fields
        addVisitor->saveInput(currentMedia);
        emit mediaCreated(currentMedia);
    });

    // in order for the keyPressEvent to work, we need to set focus policy
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

void CreateMediaWidget::chooseImage() {
    const auto selectedImagePath =
        QFileDialog::getOpenFileName(this, "Choose Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!selectedImagePath.isEmpty()) {
        imagePathEdit->setText(selectedImagePath);
    }
}

QVBoxLayout* CreateMediaWidget::getContentLayout() const {
    return contentLayout;
}

void CreateMediaWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // if the create button is enabled, simulate a click
        if (createButton && createButton->isEnabled()) {
            createButton->click();
        }
    } else {
        // default key press handling
        QWidget::keyPressEvent(event);
    }
}

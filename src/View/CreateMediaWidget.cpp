#include "CreateMediaWidget.h"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
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

    // note: clearing is not to be done here, rightDynamicWidget
    // will clear the layout before setting this widget

    // to add image from dialog
    contentLayout->addWidget(new QLabel("Image Path:"));
    imagePathEdit = new QLineEdit();
    imagePathEdit->setReadOnly(true);
    contentLayout->addWidget(imagePathEdit);
    chooseButton = new QPushButton("Choose");
    contentLayout->addWidget(chooseButton);
    connect(chooseButton, &QPushButton::clicked, this, &CreateMediaWidget::chooseImage);

    // visualize the input fields for the selected concrete media (with safe memory management)
    addVisitor = std::make_unique<AddVisitor>(contentLayout);
    media->accept(addVisitor.get());

    createButton = new QPushButton("Create", this);  // Assign to member variable
    contentLayout->addWidget(createButton);

    contentWidget->setLayout(contentLayout);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    setLayout(layout);

    connect(createButton, &QPushButton::clicked, this, [this]() {
        if (currentMedia) {
            currentMedia->setImagePath(imagePathEdit->text());
            addVisitor->saveInput(currentMedia);  // no need to manually delete addVisitor
            emit mediaCreated(currentMedia);
        }
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

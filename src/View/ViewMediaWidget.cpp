#include "ViewMediaWidget.h"

#include <QCoreApplication>
#include <QDir>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "../Model/EditVisitor.h"
#include "../Model/ViewVisitor.h"

ViewMediaWidget::ViewMediaWidget(Media* media, QWidget* parent) : QWidget(parent), media(media) {
    mainLayout = new QVBoxLayout(this);

    // adding the edit button and connecting it to the editVisitor
    const auto topLayout = new QHBoxLayout();
    editButton = new QPushButton("Edit", this);
    topLayout->addStretch();  // push the button to the right
    topLayout->addWidget(editButton);
    connect(editButton, &QPushButton::clicked, this, &ViewMediaWidget::onEditButtonClicked);
    mainLayout->addLayout(topLayout);

    auto scrollArea = new QScrollArea(this);
    auto contentWidget = new QWidget(scrollArea);
    auto contentLayout = new QVBoxLayout(contentWidget);

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    // image loading
    const QString appDirPath = QCoreApplication::applicationDirPath();
    QDir dir(appDirPath);
    dir.cdUp();
    dir.cdUp();
    const QString imgPath = dir.filePath("media") + "/" + media->getId() + ".jpg";
    qDebug() << "imgPath:" << imgPath;

    QImage image(imgPath);
    if (image.isNull()) {
        image.load(":default.jpg");
        qDebug() << "image is null";
    }
    QLabel* imageLabel = new QLabel;
    imageLabel->setPixmap(QPixmap::fromImage(image).scaled(300, 300, Qt::KeepAspectRatio));
    imageLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(imageLabel);

    // visit media for dynamic visualization, with safe memory management
    viewVisitor = std::make_unique<ViewVisitor>(contentLayout);
    media->accept(viewVisitor.get());
}

void ViewMediaWidget::onEditButtonClicked() {
    editButton->setEnabled(false);  // in order to avoid multiple edits
    saveButton = new QPushButton("Save Changes", this);
    editVisitor = std::make_unique<EditVisitor>(mainLayout, saveButton);
    media->accept(editVisitor.get());

    // to allow keyboard shortcuts
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    connect(editVisitor.get(), &EditVisitor::mediaEdited, this, [=](Media* media) {
        emit mediaEdited(media);
        editButton->setEnabled(true);  // re-enable the edit button
        // edit visitor will automatically be destroyed
    });
}

void ViewMediaWidget::keyPressEvent(QKeyEvent* event) {
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && saveButton && saveButton->isEnabled()) {
        saveButton->click();
    } else {
        QWidget::keyPressEvent(event);
    }
}

ViewMediaWidget::~ViewMediaWidget() = default;

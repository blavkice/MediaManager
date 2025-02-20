#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QFileDialog>
#include <QLabel>
#include "CreateMediaWidget.h"
#include "../Model/AddVisitor.h"

CreateMediaWidget::CreateMediaWidget(QWidget* parent, Media* media)
    : QWidget(parent), currentMedia(media) {
    auto layout = new QVBoxLayout(this);
    auto scrollArea = new QScrollArea(this);
    auto contentWidget = new QWidget(scrollArea);
    contentLayout = new QVBoxLayout(contentWidget);

    // to clear if different kind of media is chosen before adding the previous one
    QLayoutItem* item;
    while ((item = contentLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // to add image from dialog
    contentLayout->addWidget(new QLabel("Image Path:"));
    imagePathEdit = new QLineEdit();
    imagePathEdit->setReadOnly(true);
    contentLayout->addWidget(imagePathEdit);
    chooseButton = new QPushButton("Choose");
    contentLayout->addWidget(chooseButton);
    connect(chooseButton, &QPushButton::clicked, this, &CreateMediaWidget::chooseImage);

    // visualize the input fields for the selected concrete media
    addVisitor = new AddVisitor(contentLayout);
    media->accept(addVisitor);

    const auto createButton = new QPushButton("Create", this);
    contentLayout->addWidget(createButton);

    contentWidget->setLayout(contentLayout);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    setLayout(layout);

    connect(createButton, &QPushButton::clicked, this, [this]() {
        if (currentMedia) {
            currentMedia->setImagePath(imagePathEdit->text());
            addVisitor->saveInput(currentMedia);
            emit mediaCreated(currentMedia);
            delete addVisitor;
        }
    });
}

void CreateMediaWidget::chooseImage() {
    const auto selectedImagePath = QFileDialog::getOpenFileName(this, "Choose Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!selectedImagePath.isEmpty()) {
        imagePathEdit->setText(selectedImagePath);
    }
}

QVBoxLayout* CreateMediaWidget::getContentLayout() const {
    return contentLayout;
}

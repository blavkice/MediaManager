#include "ViewMediaWidget.h"
#include "../Model/ViewVisitor.h"
#include "../Model/EditVisitor.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>

ViewMediaWidget::ViewMediaWidget(Media* media, QWidget* parent)
    : QWidget(parent), media(media) {
    mainLayout = new QVBoxLayout(this);

    // adding the edit button and connecting it to the editVisitor
    const auto topLayout = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("Edit", this);
    topLayout->addStretch(); // Push the button to the right
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

    // Visit media for dynamic visualization
    viewVisitor = new ViewVisitor(contentLayout);
    media->accept(viewVisitor);
}

void ViewMediaWidget::onEditButtonClicked() {
    auto* editVisitor = new EditVisitor(mainLayout);
    media->accept(editVisitor);
    connect(editVisitor, &EditVisitor::mediaEdited, this, [=](Media* media) {
        emit mediaEdited(media); // emit to the main window
    });
}

ViewMediaWidget::~ViewMediaWidget() {
    delete viewVisitor;
}

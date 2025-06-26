#include "ViewMediaWidget.h"
#include "../Model/ViewVisitor.h"
#include "../Model/EditVisitor.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QDir>
#include <QCoreApplication>

ViewMediaWidget::ViewMediaWidget(Media* media, QWidget* parent)
    : QWidget(parent), media(media) {
    mainLayout = new QVBoxLayout(this);

    // adding the edit button and connecting it to the editVisitor
    const auto topLayout = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("Edit", this);
    topLayout->addStretch(); // push the button to the right
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
    auto editVisitor = new EditVisitor(mainLayout); // qt parent takes ownership
    media->accept(editVisitor);

    connect(editVisitor, &EditVisitor::mediaEdited, this, [=](Media* media) {
        emit mediaEdited(media);
    });
}

ViewMediaWidget::~ViewMediaWidget() = default;

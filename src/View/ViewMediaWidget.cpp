#include "ViewMediaWidget.h"
#include "../Model/ViewVisitor.h"
#include <QVBoxLayout>
#include <QScrollArea>

ViewMediaWidget::ViewMediaWidget(Media* media, QWidget* parent)
    : QWidget(parent), media(media) {
    mainLayout = new QVBoxLayout(this);
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

ViewMediaWidget::~ViewMediaWidget() {
    delete viewVisitor;
}

#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include "CreateMediaWidget.h"
#include "../Model/AddVisitor.h"

CreateMediaWidget::CreateMediaWidget(QWidget* parent, Media* media)
    : QWidget(parent), currentMedia(media) {
    auto layout = new QVBoxLayout(this);
    auto scrollArea = new QScrollArea(this);
    auto contentWidget = new QWidget(scrollArea);
    contentLayout = new QVBoxLayout(contentWidget);

    auto createButton = new QPushButton("Create", this);
    contentLayout->addWidget(createButton);

    contentWidget->setLayout(contentLayout);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    setLayout(layout);

    connect(createButton, &QPushButton::clicked, this, [this]() {
        if (currentMedia) {
            AddVisitor visitor(contentLayout);
            currentMedia->accept(&visitor);
            visitor.saveInput(currentMedia);
            emit mediaCreated(currentMedia);
        }
    });
}

QVBoxLayout* CreateMediaWidget::getContentLayout() const {
    return contentLayout;
}

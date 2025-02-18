#include "CreateMediaWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

CreateMediaWidget::CreateMediaWidget(QWidget* parent) : QWidget(parent), currentMedia(nullptr) {
    auto layout = new QVBoxLayout(this);
    auto createButton = new QPushButton("Add media", this);
    layout->addWidget(createButton);

    connect(createButton, &QPushButton::clicked, this, [this]() {
        if (currentMedia) {
            emit mediaCreated(currentMedia);
        }
    });
}

void CreateMediaWidget::setMedia(Media* media) {
    // for UI update
    currentMedia = media;
}

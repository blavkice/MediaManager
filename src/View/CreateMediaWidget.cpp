#include "CreateMediaWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

CreateMediaWidget::CreateMediaWidget(QWidget* parent, Media* media)
    : QWidget(parent),
      currentMedia(media) {
    auto layout = new QVBoxLayout(this);

    auto createButton = new QPushButton("Add media", this);
    layout->addWidget(createButton);

    connect(createButton, &QPushButton::clicked, this, [this]() {
        if (currentMedia) {
            emit mediaCreated(currentMedia);
        }
    });
}

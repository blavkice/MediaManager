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

ViewMediaWidget::ViewMediaWidget(Media* media, QWidget* parent)
    : QWidget(parent),
      media(media),
      viewVisitor(nullptr),
      editVisitor(nullptr),
      saveButton(nullptr),
      editButton(nullptr),
      imageLabel(nullptr),
      editContainer(nullptr) {
    mainLayout = new QVBoxLayout(this);
    showMediaView();
}

void ViewMediaWidget::showMediaView() {
    // clean layout (remove all widgets and visitors)
    QLayoutItem* child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (auto w = child->widget()) w->deleteLater();
        delete child;
    }
    if (editContainer) {
        editContainer->deleteLater();
        editContainer = nullptr;
    }

    // add edit button and connect
    const auto topLayout = new QHBoxLayout();
    editButton = new QPushButton("Edit", this);
    topLayout->addStretch();  // push the button to the right
    topLayout->addWidget(editButton);
    connect(editButton, &QPushButton::clicked, this, [this]() {
        editButton->setVisible(false);
        showEditView();
    });
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
    const QString imgPath = dir.filePath("media") + "/" + media->getId() + ".jpg";
    QImage image(imgPath);
    if (image.isNull()) image.load(":default.jpg");

    imageLabel = new QLabel;
    imageLabel->setPixmap(QPixmap::fromImage(image).scaled(300, 300, Qt::KeepAspectRatio));
    imageLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(imageLabel);

    // visit media for dynamic visualization, with safe memory management
    viewVisitor = std::make_unique<ViewVisitor>(contentLayout);
    media->accept(viewVisitor.get());
}

void ViewMediaWidget::showEditView() {
    // clean layout (remove all widgets and visitors), including view!
    QLayoutItem* child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (auto w = child->widget()) w->deleteLater();
        delete child;
    }
    if (imageLabel) {
        imageLabel->deleteLater();
        imageLabel = nullptr;
    }
    viewVisitor.reset();

    // edit visitor container
    editContainer = new QWidget(this);
    auto editLayout = new QVBoxLayout(editContainer);

    saveButton = new QPushButton("Save Changes", editContainer);
    editVisitor = std::make_unique<EditVisitor>(editLayout, saveButton);

    // edit visitor will add its own widgets to the layout
    media->accept(editVisitor.get());

    mainLayout->addWidget(editContainer);

    connect(editVisitor.get(), &EditVisitor::mediaEdited, this, [this](Media* m) {
        emit mediaEdited(m);
        // after editing, return to view mode
        showMediaView();
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

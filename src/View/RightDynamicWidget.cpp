#include "RightDynamicWidget.h"

#include <QFontDatabase>
#include <QLabel>
#include <QVBoxLayout>

#include "ViewMediaWidget.h"

void RightDynamicWidget::clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->setParent(nullptr);
            widget->deleteLater();  // safe qt deletion
        }
        delete item;
    }
}

// note: clearLayout but without deleting the layout itself
void RightDynamicWidget::clear() {
    if (layout()) {
        clearLayout(layout());
    }
    currentWidget = nullptr;
}

RightDynamicWidget::RightDynamicWidget(QWidget* parent) : QWidget(parent), currentWidget(nullptr) {
    auto layout = new QVBoxLayout(this);
    auto welcomeLabel = new QLabel("Welcome to MediaManager!", this);

    const int fontId = QFontDatabase::addApplicationFont(":/Fonts/AceSansExtrabold.ttf");
    if (fontId != -1) {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(fontFamily);
        font.setPointSize(30);
        welcomeLabel->setFont(font);
    }

    layout->addWidget(welcomeLabel);
    setLayout(layout);
}

void RightDynamicWidget::setWidget(QWidget* widget) {
    clear();
    currentWidget = widget;
    if (currentWidget) {
        layout()->addWidget(currentWidget);
        currentWidget->show();
    }
}

void RightDynamicWidget::setMediaCreated() {
    clear();
    setWidget(new QLabel("Media created!", this));
}

void RightDynamicWidget::viewMedia(Media* media) {
    clear();
    const auto viewMediaWidget = new ViewMediaWidget(media, this);
    setWidget(viewMediaWidget);

    // after a media is viewed it can be edited, and so we need to connect the mediaEdited to the main window
    connect(viewMediaWidget, &ViewMediaWidget::mediaEdited, this, [this, media]() { emit mediaEdited(media); });
}

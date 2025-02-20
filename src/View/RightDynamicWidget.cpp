#include <QVBoxLayout>
#include <QLabel>
#include <QFontDatabase>
#include "RightDynamicWidget.h"
#include "ViewMediaWidget.h"

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
    if(currentWidget) {
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
}

void RightDynamicWidget::clear() {
    // to delete recursively all the widgets in the layout
    QLayoutItem* item;
    while ((item = layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    currentWidget = nullptr;
}

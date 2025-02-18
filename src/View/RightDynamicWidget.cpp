#include <QVBoxLayout>
#include <QLabel>
#include <QFontDatabase>
#include "RightDynamicWidget.h"

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
    if(currentWidget) {
        layout()->removeWidget(currentWidget);
        currentWidget->hide();
    }
    currentWidget = widget;
    if(currentWidget) {
        layout()->addWidget(currentWidget);
        currentWidget->show();
    }
}

void RightDynamicWidget::setMediaCreated() {
    if(currentWidget) {
        layout()->removeWidget(currentWidget);
        currentWidget->hide();
    }
    currentWidget = new QLabel("Media created!", this);
    layout()->addWidget(currentWidget);
    currentWidget->show();
}

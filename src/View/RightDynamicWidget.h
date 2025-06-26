#ifndef RIGHTDYNAMICWIDGET_H
#define RIGHTDYNAMICWIDGET_H
#include <QWidget>
#include "../Model/Media.h"

class RightDynamicWidget : public QWidget {
    Q_OBJECT
private:
    QWidget* currentWidget;
    static void clearLayout(QLayout* layout);
public:
    explicit RightDynamicWidget(QWidget* parent = nullptr);
    void setWidget(QWidget* widget);
    void setMediaCreated();
    void viewMedia(Media* media);
    void clear();
signals:
    void mediaEdited(Media* media);
};

#endif //RIGHTDYNAMICWIDGET_H

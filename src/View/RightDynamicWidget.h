#ifndef RIGHTDYNAMICWIDGET_H
#define RIGHTDYNAMICWIDGET_H
#include <QWidget>

class RightDynamicWidget : public QWidget {
    Q_OBJECT
private:
    QWidget* currentWidget;
public:
    explicit RightDynamicWidget(QWidget* parent = nullptr);
    void setWidget(QWidget* widget);
    void setMediaCreated();
};

#endif //RIGHTDYNAMICWIDGET_H

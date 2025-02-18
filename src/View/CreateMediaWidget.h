#ifndef CREATEMEDIAWIDGET_H
#define CREATEMEDIAWIDGET_H
#include <QWidget>
#include "../Model/Media.h"

class CreateMediaWidget : public QWidget {
    Q_OBJECT
private:
    Media* currentMedia;
    QVBoxLayout* contentLayout;
public:
    explicit CreateMediaWidget(QWidget* parent = nullptr, Media* media = nullptr);
    QVBoxLayout* getContentLayout() const;
signals:
    void mediaCreated(Media* media);
};

#endif //CREATEMEDIAWIDGET_H

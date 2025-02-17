#ifndef CREATEMEDIAWIDGET_H
#define CREATEMEDIAWIDGET_H
#include <QWidget>
#include "../Model/Media.h"

class CreateMediaWidget : public QWidget {
    Q_OBJECT
private:
    Media* currentMedia;
public:
    explicit CreateMediaWidget(QWidget* parent = nullptr);
    void setMedia(Media* media);
signals:
    void mediaCreated(Media* media);
};

#endif //CREATEMEDIAWIDGET_H

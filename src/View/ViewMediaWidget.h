#ifndef VIEWMEDIAWIDGET_H
#define VIEWMEDIAWIDGET_H
#include <QWidget>
#include <QVBoxLayout>
#include "../Model/ViewVisitor.h"
#include "../Model/Media.h"

class ViewMediaWidget : public QWidget {
    Q_OBJECT
private:
    Media* media;
    QVBoxLayout* mainLayout;
    ViewVisitor* viewVisitor;
public:
    explicit ViewMediaWidget(Media* media, QWidget* parent = nullptr);
    virtual ~ViewMediaWidget();
};

#endif // VIEWMEDIAWIDGET_H

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
    void onEditButtonClicked();
public:
    explicit ViewMediaWidget(Media* media, QWidget* parent = nullptr);
    ~ViewMediaWidget() override;
signals:
    void mediaEdited(Media* media);
};

#endif // VIEWMEDIAWIDGET_H

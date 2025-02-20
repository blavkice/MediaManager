#ifndef CREATEMEDIAWIDGET_H
#define CREATEMEDIAWIDGET_H
#include <QWidget>
#include "../Model/Media.h"
#include "../Model/AddVisitor.h"

class CreateMediaWidget : public QWidget {
    Q_OBJECT
private:
    Media* currentMedia;
    QVBoxLayout* contentLayout;
    AddVisitor* addVisitor;
    // to add image from dialog
    QLineEdit* imagePathEdit;
    QPushButton* chooseButton;
    void chooseImage();
public:
    explicit CreateMediaWidget(QWidget* parent = nullptr, Media* media = nullptr);
    QVBoxLayout* getContentLayout() const;
signals:
    void mediaCreated(Media* media);
};

#endif //CREATEMEDIAWIDGET_H

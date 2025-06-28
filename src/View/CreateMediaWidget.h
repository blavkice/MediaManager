#ifndef CREATEMEDIAWIDGET_H
#define CREATEMEDIAWIDGET_H
#include <QWidget>
#include <QKeyEvent>
#include "../Model/Media.h"
#include "../Model/AddVisitor.h"

class CreateMediaWidget : public QWidget {
    Q_OBJECT
private:
    Media* currentMedia;
    QVBoxLayout* contentLayout;
    std::unique_ptr<AddVisitor> addVisitor;
    // to add image from dialog
    QLineEdit* imagePathEdit;
    QPushButton* chooseButton;
    QPushButton* createButton; // it is a member variable to allow the keyPressEvent
    void chooseImage();
public:
    explicit CreateMediaWidget(QWidget* parent = nullptr, Media* media = nullptr);
    QVBoxLayout* getContentLayout() const;
protected:
    void keyPressEvent(QKeyEvent *event) override;
signals:
    void mediaCreated(Media* media);
};

#endif //CREATEMEDIAWIDGET_H

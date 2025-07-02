#ifndef VIEWMEDIAWIDGET_H
#define VIEWMEDIAWIDGET_H
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "../Model/EditVisitor.h"
#include "../Model/Media.h"
#include "../Model/ViewVisitor.h"

class ViewMediaWidget : public QWidget {
    Q_OBJECT
   private:
    Media* media;
    QVBoxLayout* mainLayout;
    std::unique_ptr<ViewVisitor> viewVisitor;
    std::unique_ptr<EditVisitor> editVisitor;
    QPushButton* saveButton;
    QPushButton* editButton;
    QLabel* imageLabel;
    QWidget* editContainer;  // to help clean layout when viewing/editing

    void showMediaView();
    void showEditView();

   protected:
    void keyPressEvent(QKeyEvent* event) override;

   public:
    explicit ViewMediaWidget(Media* media, QWidget* parent = nullptr);
    ~ViewMediaWidget() override;
   signals:
    void mediaEdited(Media* media);
};

#endif  // VIEWMEDIAWIDGET_H

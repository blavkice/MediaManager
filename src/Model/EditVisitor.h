#ifndef EDITVISITOR_H
#define EDITVISITOR_H
#include <QLayout>
#include <QPushButton>
#include <QWidget>

#include "AddVisitor.h"

class EditVisitor : public AddVisitor {
    Q_OBJECT
   private:
    Media* currentMedia = nullptr;
    QPushButton* saveButton;

   public:
    explicit EditVisitor(QLayout* layout, QPushButton* saveButton);
    void visit(Media* media) override;
    void saveChanges(Media* media);
   signals:
    void mediaEdited(Media* media);
};

#endif  // EDITVISITOR_H

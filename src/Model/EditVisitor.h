#ifndef EDITVISITOR_H
#define EDITVISITOR_H
#include <QPushButton>
#include <QLayout>
#include <QWidget>
#include "AddVisitor.h"

class EditVisitor : public AddVisitor {
    Q_OBJECT
private:
    QPushButton* saveButton;
    Media* currentMedia = nullptr;
public:
    explicit EditVisitor(QLayout* layout, QWidget* parent = nullptr);
    void visit(Media* media) override;
    void saveChanges(Media* media);
signals:
    void mediaEdited(Media* media);
};

#endif //EDITVISITOR_H

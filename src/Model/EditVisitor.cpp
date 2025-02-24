#include "EditVisitor.h"
#include <QMessageBox>

EditVisitor::EditVisitor(QLayout* layout, QWidget* parent)
    : AddVisitor(layout, parent) {
    saveButton = new QPushButton("Save Changes");
    layout->addWidget(saveButton);

    QObject::connect(saveButton, &QPushButton::clicked, this, [this]() {
        saveChanges(currentMedia);
    });
}

void EditVisitor::visit(Media* media) {
    currentMedia = media;
    AddVisitor::visit(media); // to prefill fields
    saveButton->setVisible(true);
}

void EditVisitor::saveChanges(Media* media) {
    if (saveInput(media)) {
        QMessageBox::information(nullptr, "Success", "Media updated successfully!");
        emit mediaEdited(media);
    } else {
        QMessageBox::warning(nullptr, "Error", "Failed to update media. Check inputs.");
    }
}

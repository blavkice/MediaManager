#include "EditVisitor.h"

#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>
#include <QString>
#include <QMessageBox>

EditVisitor::EditVisitor(QLayout* layout, QPushButton* saveButton)
    : AddVisitor(layout), saveButton(saveButton) {
    // saveButton is passed from the ViewMediaWidget in order to allow the keyboard shortcut to work
    layout->addWidget(saveButton);

    // Bottone per cambiare immagine
    auto changeImgBtn = new QPushButton("Change Image", layout->parentWidget());
    layout->addWidget(changeImgBtn);

    QObject::connect(changeImgBtn, &QPushButton::clicked, this, [this]() {
        if (!currentMedia) return;

        QString fileName = QFileDialog::getOpenFileName(
            nullptr, "Select Image", QCoreApplication::applicationDirPath(),
            "Images (*.png *.jpg *.jpeg *.bmp *.gif)"
        );
        if (fileName.isEmpty())
            return;

        // Assicurati che la cartella media esista!
        QString mediaDirPath = QCoreApplication::applicationDirPath() + "/media";
        QDir mediaDir(mediaDirPath);
        if (!mediaDir.exists()) {
            mediaDir.mkpath(".");
        }

        QString destPath = mediaDirPath + "/" + currentMedia->getId() + ".jpg";

        qDebug() << "Copying from:" << fileName << "to:" << destPath;

        // Rimuovi la vecchia immagine e copia la nuova
        QFile::remove(destPath);
        if (!QFile::copy(fileName, destPath)) {
            QMessageBox::warning(nullptr, "Error", "Failed to copy the image!");
        }
    });


    QObject::connect(saveButton, &QPushButton::clicked, this, [this]() { saveChanges(currentMedia); });
}

void EditVisitor::visit(Media* media) {
    currentMedia = media;
    AddVisitor::visit(media);  // to prefill fields
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

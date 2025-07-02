#include "MenuBar.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

MenuBar::MenuBar(QWidget* parent) : QMenuBar(parent) {
    actionsMenu = new QMenu(tr("Actions"), this);
    importAction = new QAction(tr("Import"), this);
    exportAction = new QAction(tr("Export"), this);
    saveAction = new QAction(tr("Save"), this);

    actionsMenu->addAction(saveAction);
    actionsMenu->addAction(importAction);
    actionsMenu->addAction(exportAction);
    addMenu(actionsMenu);

    connect(saveAction, &QAction::triggered, this, &MenuBar::onSaveActionTriggered);
    connect(importAction, &QAction::triggered, this, &MenuBar::onImportActionTriggered);
    connect(exportAction, &QAction::triggered, this, &MenuBar::onExportActionTriggered);
}

void MenuBar::importLastDefaultMedia() {
    if (jsonVisitor) {
        QString appDirPath = QCoreApplication::applicationDirPath();
        QDir dir(appDirPath);
        QString filePath = dir.filePath("saves/last.json");
        qDebug() << "Importing last default media from:" << filePath;
        if (jsonVisitor->importFromFile(filePath)) {
            emit mediaImported();
        } else {
            QMessageBox::warning(this, tr("Import"), tr("Default import failed or this is the first run!"));
        }
    }
}

void MenuBar::replaceLastDefaultMedia(const QString& filePath) {
    const QFile file(filePath);
    if (!file.exists() || !filePath.endsWith(".json")) return;

    const QString appDirPath = QCoreApplication::applicationDirPath();
    QDir dir(appDirPath);
    const QString lastJsonPath = dir.filePath("saves/last.json");

    if (QFile::remove(lastJsonPath)) QFile::copy(filePath, lastJsonPath);
}

void MenuBar::onImportActionTriggered() {
    if (jsonVisitor) {
        const QString path = QFileDialog::getOpenFileName(this, tr("Import JSON"), "", tr("JSON Files (*.json)"));
        if (jsonVisitor->importFromFile(path)) {
            emit mediaImported();
            replaceLastDefaultMedia(path);
        } else {
            QMessageBox::warning(this, tr("Import"), tr("Import failed!"));
        }
    }
}

void MenuBar::onExportActionTriggered() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export JSON"), "", tr("JSON Files (*.json)"));
    if (!filePath.isEmpty()) {
        if (jsonVisitor->exportToFile(filePath)) {
            replaceLastDefaultMedia(filePath);
            QMessageBox::information(this, tr("Export"), tr("Export successful!"));
        } else {
            QMessageBox::warning(this, tr("Export"), tr("Export failed!"));
        }
    }
}

void MenuBar::onSaveActionTriggered() {
    if (!jsonVisitor) return;

    // save the current state to last.json in the saves directory
    const QString appDirPath = QCoreApplication::applicationDirPath();
    QDir dir(appDirPath);
    const QString lastJsonPath = dir.filePath("saves/last.json");

    if (jsonVisitor->exportToFile(lastJsonPath)) {
        QMessageBox::information(this, tr("Save"), tr("Data saved correctly!"));
    } else {
        QMessageBox::warning(this, tr("Save"), tr("Save failed."));
    }
}

void MenuBar::setJSONVisitor(JSONVisitor* jsonVisitor) {
    this->jsonVisitor = jsonVisitor;
}

QAction* MenuBar::getImportAction() const {
    return importAction;
}

QAction* MenuBar::getExportAction() const {
    return exportAction;
}

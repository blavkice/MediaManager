#include "MenuBar.h"
#include <QFileDialog>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

MenuBar::MenuBar(QWidget* parent)
    : QMenuBar(parent) {
    actionsMenu = new QMenu(tr("Actions"), this);
    exit = new QAction(tr("Exit MediaManager"), this);
    importAction = new QAction(tr("Import"), this);
    exportAction = new QAction(tr("Export"), this);

    actionsMenu->addAction(importAction);
    actionsMenu->addAction(exportAction);
    actionsMenu->addAction(exit);
    addMenu(actionsMenu);

    // in order to show the personalized exit button in macOS
    exit->setMenuRole(QAction::NoRole);

    connect(importAction, &QAction::triggered, this, &MenuBar::onImportActionTriggered);
    connect(exportAction, &QAction::triggered, this, &MenuBar::onExportActionTriggered);
}

// IMPORTANT NOTE: it is working because exec is being started from MM/src/build-debug, adjust accordingly!
void MenuBar::importLastDefaultMedia() {
    if (jsonVisitor) {
        QString appDirPath = QCoreApplication::applicationDirPath();
        QDir dir(appDirPath);
        dir.cdUp(); dir.cdUp();
        QString filePath = dir.filePath("saves/last.json");
        qDebug() << "Importing last default media from:" << filePath;
        if (jsonVisitor->importFromFile(filePath)) {
            emit mediaImported();
        } else {
            QMessageBox::warning(this, tr("Import"), tr("Default import failed!"));
        }
    }
}

void MenuBar::replaceLastDefaultMedia(const QString& filePath) {
    const QFile file(filePath);
    if (!file.exists() || !filePath.endsWith(".json")) return;

    const QString appDirPath = QCoreApplication::applicationDirPath();
    QDir dir(appDirPath);
    dir.cdUp(); dir.cdUp();
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

void MenuBar::setJSONVisitor(JSONVisitor* jsonVisitor) {
    this->jsonVisitor = jsonVisitor;
}

QAction* MenuBar::getExit() const {
    return exit;
}

QAction* MenuBar::getImportAction() const {
    return importAction;
}

QAction* MenuBar::getExportAction() const {
    return exportAction;
}

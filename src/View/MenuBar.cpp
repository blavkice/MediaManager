#include "MenuBar.h"
#include <QFileDialog>
#include <QMessageBox>

MenuBar::MenuBar(QWidget* parent) : QMenuBar(parent), jsonEditor(new JSONEditor) {
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

void MenuBar::onImportActionTriggered() {
    if (jsonEditor) {
        QList<Media*> mediaList = jsonEditor->getMediaList();
        emit mediaImported(mediaList);
    }
}

void MenuBar::onExportActionTriggered() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export JSON"), "", tr("JSON Files (*.json)"));
    if (!filePath.isEmpty()) {
        if (jsonEditor->exportToFile(filePath)) {
            QMessageBox::information(this, tr("Export"), tr("Export successful!"));
        } else {
            QMessageBox::warning(this, tr("Export"), tr("Export failed!"));
        }
    }
}

void MenuBar::setJSONVisitor(JSONEditor* jsonVisitor) {
    this->jsonEditor = jsonVisitor;
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

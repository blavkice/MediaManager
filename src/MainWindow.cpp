#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    initMenuBar();
}

void MainWindow::initMenuBar() {
    actionsMenu = menuBar->addMenu(tr("Actions"));
    // actions
    exit = new QAction(tr("Exit"), this);
    save = new QAction(tr("Save"), this);
    actionsMenu->addAction(exit);
    actionsMenu->addAction(save);
    connect(exit, &QAction::triggered, this, &MainWindow::close);
}

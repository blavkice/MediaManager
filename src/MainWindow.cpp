#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    resize(800, 600);
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    initMenuBar();
    initLayouts();
}

void MainWindow::initLayouts() {
    hMainViewLayout = new QHBoxLayout(centralWidget);
    auto vLeftWidget = new QWidget(centralWidget);
    auto searchLabel = new QLabel("Search item:", vLeftWidget);
    vLeftLayout = new QVBoxLayout(vLeftWidget);
    searchBox = new QLineEdit(vLeftWidget);
    listView = new QListView(vLeftWidget);
    gridLayout = new QGridLayout(centralWidget);

    vLeftLayout->addWidget(searchLabel);
    vLeftLayout->addWidget(searchBox);
    vLeftLayout->addWidget(listView);
    vLeftWidget->setFixedWidth(250);

    hMainViewLayout->addWidget(vLeftWidget);
    hMainViewLayout->addItem(new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hMainViewLayout->addLayout(gridLayout);
    centralWidget->setLayout(hMainViewLayout);
}

void MainWindow::initMenuBar() {
    actionsMenu = menuBar->addMenu(tr("Actions"));
    exit = new QAction(tr("Exit MediaManager"), this);
    save = new QAction(tr("Save"), this);

    actionsMenu->addAction(save);
    actionsMenu->addAction(exit);
    // in order to show the personalized exit button in MacOS
    exit->setMenuRole(QAction::NoRole);

    connect(exit, &QAction::triggered, this, &MainWindow::close);
}

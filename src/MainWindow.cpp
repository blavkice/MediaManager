#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    resize(800, 600);
    // menu bar initialization
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    initMenuBar();
    // initialize layouts: the left vertical one and the main horizontal one on the right
    hMainViewLayout = new QHBoxLayout(centralWidget);
    auto vLeftWidget = new QWidget(centralWidget);
    const auto searchLabel = new QLabel("Search item:", vLeftWidget);
    vLeftLayout = new QVBoxLayout(vLeftWidget);
    searchBox = new QLineEdit(vLeftWidget);
    listView = new QListView(vLeftWidget);
    gridLayout = new QGridLayout(centralWidget);
    // add widgets to layouts
    vLeftLayout->addWidget(searchLabel);
    vLeftLayout->addWidget(searchBox);
    vLeftLayout->addWidget(listView);
    vLeftWidget->setFixedWidth(250);
    hMainViewLayout->addWidget(vLeftWidget);
    hMainViewLayout->addItem(new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hMainViewLayout->addLayout(gridLayout);
    centralWidget->setLayout(hMainViewLayout);
}

void MainWindow::initLayouts() {
    // TBD
}

void MainWindow::initMenuBar() {
    actionsMenu = menuBar->addMenu(tr("Actions"));
    // actions
    exit = new QAction(tr("Exit MediaManager"), this);
    exit->setMenuRole(QAction::NoRole);
    save = new QAction(tr("Save"), this);
    actionsMenu->addAction(save);
    actionsMenu->addAction(exit);
    connect(exit, &QAction::triggered, this, &MainWindow::close);
}

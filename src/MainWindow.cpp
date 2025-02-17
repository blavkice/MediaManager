#include <QComboBox>
#include "MainWindow.h"
#include "Model/LiteratureClasses/Book.h"
#include "Model/LiteratureClasses/Poem.h"
#include "Model/ArticlesClasses/NewspaperArticle.h"
#include "Model/ArticlesClasses/AcademicArticle.h"
#include "View/CreateMediaWidget.h"
#include "Model/AddVisitor.h"

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    centralWidget(new QWidget(this)),
    menuBar(new QMenuBar(this)) {
    resize(800, 600);
    setMenuBar(menuBar);
    setCentralWidget(centralWidget);
    initMenuBar();
    initLayouts();
    initAddComboBox();
}

void MainWindow::initLayouts() {
    hMainViewLayout = new QHBoxLayout(centralWidget);
    // the "multifunctional" widget that will be used to display the media info
    rightInfoWidget = new QWidget(centralWidget);
    // the widget for the left "fixed" part
    auto vLeftWidget = new QWidget(centralWidget);
    // the widget containing the search box and add and remove buttons
    auto utilsWidget = new QWidget(vLeftWidget);
    auto utilsHorizontalLayout = new QHBoxLayout(utilsWidget);
    utilsWidget->setLayout(utilsHorizontalLayout);
    auto searchLabel = new QLabel("Search:", utilsWidget);
    vLeftLayout = new QVBoxLayout(vLeftWidget);
    searchBox = new QLineEdit(utilsWidget);
    addButton = new QPushButton("+", utilsWidget);
    addButton->setToolTip("add new media");
    addButton->setFixedSize(25, 25);
    removeButton = new QPushButton("-", utilsWidget);
    removeButton->setToolTip("remove selected media");
    removeButton->setFixedSize(25, 25);
    listView = new QListView(vLeftWidget);
    mediaListController = new MediaListController(listView);

    utilsHorizontalLayout->addWidget(searchLabel);
    utilsHorizontalLayout->addWidget(searchBox);
    utilsHorizontalLayout->addWidget(addButton);
    utilsHorizontalLayout->addWidget(removeButton);

    vLeftLayout->addWidget(utilsWidget);
    vLeftLayout->addWidget(listView);
    // listView->sIDelegate is already set in MediaListController
    vLeftWidget->setFixedWidth(250);

    hMainViewLayout->addWidget(vLeftWidget);
    hMainViewLayout->addItem(new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hMainViewLayout->addWidget(rightInfoWidget);
    centralWidget->setLayout(hMainViewLayout);


    //TB removed
    Book* book1 = new Book(1, "Book Title 1", "Short description for book 1, we we'll see now the displacement, i dunno if it will do a gooood job butt", "", "Author 1", "Long description for book 1, not really long TBD", 2021, 5, "Publishing House 1", 300);
    Poem* poem1 = new Poem(2, "Poem Title 1", "Short description for poem 1 Short description for poem 1 Short description for poem 1 Short description for poem 1 Short description for poem 1", "", "Author 2", "Long description for poem 1", 2019, 4, "City 1");
    Book* book2 = new Book(3, "Book Title 2", "Short description for book 2", "", "Author 3", "Long description for book 2", 2020, 3, "Publishing House 2", 250);
    Poem* poem2 = new Poem(4, "Poem Title 2", "Short description for poem 2", "", "Author 4", "Long description for poem 2", 2018, 5, "City 2");
    Book* book3 = new Book(5, "Book Title 3", "Short description for book 3", "", "Author 5", "Long description for book 3", 2022, 4, "Publishing House 3", 400);

    mediaListController->addMedia(book1);
    mediaListController->addMedia(poem1);
    mediaListController->addMedia(book2);
    mediaListController->addMedia(poem2);
    mediaListController->addMedia(book3);
}


void MainWindow::initAddComboBox() {
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    addComboBox = new QComboBox(this);
    addComboBox->addItem("Select media type:");
    addComboBox->addItem("Book");
    addComboBox->addItem("Poem");
    addComboBox->addItem("Academic Article");
    addComboBox->addItem("Newspaper Article");
    addComboBox->setVisible(false);
    connect(addComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::onComboBoxActivated);
}

void MainWindow::initMenuBar() {
    actionsMenu = menuBar->addMenu(tr("Actions"));
    exit = new QAction(tr("Exit MediaManager"), this);
    save = new QAction(tr("Save"), this);

    actionsMenu->addAction(save);
    actionsMenu->addAction(exit);
    // in order to show the personalized exit button in macOS
    exit->setMenuRole(QAction::NoRole);

    connect(exit, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::onAddButtonClicked() const {
    QPoint pos = addButton->mapToGlobal(QPoint(0, addButton->height()));
    pos = mapFromGlobal(pos);
    addComboBox->move(pos);
    addComboBox->setVisible(true);
    addComboBox->showPopup();
}

void MainWindow::onComboBoxActivated(const int index) {
    onMediaSelected(index);
    addComboBox->setVisible(false);
}

void MainWindow::onMediaSelected(const int index) {
    if (index < 1) return;
    Media* media = nullptr;
    switch (index) {
        case 1: media = new Book(); break;
        case 2: media = new Poem(); break;
        case 3: media = new AcademicArticle(); break;
        case 4: media = new NewspaperArticle(); break;
    }
    if (media) {
        auto createMediaWidget = new CreateMediaWidget(rightInfoWidget);
        AddVisitor addVisitor(createMediaWidget);
        media->accept(&addVisitor);
        rightInfoWidget->hide();
        if (rightInfoWidget->layout()) {
            QLayoutItem* item;
            while ((item = rightInfoWidget->layout()->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete rightInfoWidget->layout();
        }
        rightInfoWidget->setParent(nullptr);
        rightInfoWidget = createMediaWidget;
        hMainViewLayout->addWidget(rightInfoWidget);
        rightInfoWidget->show();
    }
}

void MainWindow::onMediaCreated(Media* media) {
    mediaListController->addMedia(media);
    rightInfoWidget->hide();
    rightInfoWidget = new QWidget(centralWidget);
    rightInfoWidget->setLayout(new QVBoxLayout());
    rightInfoWidget->show();
}

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
    menuBar(new MenuBar(this)) {
    resize(800, 600);
    initLayouts();
    setMenuBar(menuBar);
    setCentralWidget(centralWidget);
    initAddComboBox();
    // json visitor for the import/export actions
    auto jsonVisitor = new JSONVisitor(mediaListController);
    menuBar->setJSONVisitor(jsonVisitor);
}

void MainWindow::initLayouts() {
    hMainViewLayout = new QHBoxLayout(centralWidget);
    // the "multifunctional" widget that will be used to display the media info
    rightInfoWidget = new RightDynamicWidget(centralWidget);
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
    hMainViewLayout->addWidget(rightInfoWidget);
    rightInfoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    centralWidget->setLayout(hMainViewLayout);


    //TB removed
    Book* book1 = new Book(1, "Book Title 1", "Short description for book 1, we we'll see now the displacement, i dunno if it will do a gooood job butt", "", "Author 1", "Long description for book 1, not really long TBD", 2021, 5, "Publishing House 1", 300);
    Poem* poem1 = new Poem(2, "Poem Title 1", "Short description for poem 1 Short description for poem 1 Short description for poem 1 Short description for poem 1 Short description for poem 1", "", "Author 2", "Long description for poem 1", 2019, 4, "City 1");
    Book* book2 = new Book(3, "Book Title 2", "Short description for book 2", "", "Author 3", "Long description for book 2", 2020, 3, "Publishing House 2", 250);
    Poem* poem2 = new Poem(4, "Poem Title 2", "Short description for poem 2", "", "Author 4", "Long description for poem 2", 2018, 5, "City 2");
    Book* book3 = new Book(5, "Book Title 3", "Short description for book 3", "", "Author 5", "Long description for book 3", 2022, 4, "Publishing House 3", 400);

    mediaListController->addMedia(book1);
    mediaListController->addMedia(poem1);
    mediaListController->addMedia(book3);
}

void MainWindow::initAddComboBox() {
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    addComboBox = new QComboBox(this);
    addComboBox->addItem("Book");
    addComboBox->addItem("Poem");
    addComboBox->addItem("Academic Article");
    addComboBox->addItem("Newspaper Article");
    addComboBox->setVisible(false);
    connect(addComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::onComboBoxActivated);
}

void MainWindow::onAddButtonClicked() const {
    QPoint pos = addButton->mapToGlobal(QPoint(0, addButton->height()));
    pos = mapFromGlobal(pos);
    addComboBox->move(pos);
    addComboBox->setVisible(true);
    addComboBox->showPopup();
}

void MainWindow::onComboBoxActivated(const int index) const {
    onMediaSelected(index);
    addComboBox->setVisible(false);
}

void MainWindow::onMediaSelected(const int index) const {
    if (index < 0) return;
    Media* media = nullptr;
    switch (index) {
        case 0: media = new Book(); break;
        case 1: media = new Poem(); break;
        case 2: media = new AcademicArticle(); break;
        case 3: media = new NewspaperArticle(); break;
    }
    const auto createMediaWidget = new CreateMediaWidget(rightInfoWidget, media);

    connect(createMediaWidget, &CreateMediaWidget::mediaCreated, this, &MainWindow::onMediaCreated);

    rightInfoWidget->setWidget(createMediaWidget);
}

void MainWindow::onMediaCreated(Media* media) const {
    mediaListController->addMedia(media);
    rightInfoWidget->setMediaCreated();
    // the clear up is done by rightInfoWidget after a widget is set: the previous widget is deleted from heap
    // and so createMediaWidget is deleted correctly everytime
}

#include <QComboBox>
#include <QMessageBox>
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

    // delete button is activated only if a media is selected
    connect(mediaListController, &MediaListController::elementSelected, this, &MainWindow::updateDeleteButtonState);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveButtonClicked);

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
    Book* book1 = new Book(1, "To Kill a Mockingbird", "A novel about the serious issues of rape and racial inequality.", "", "Harper Lee", "Published in 1960, it was immediately successful, winning the Pulitzer Prize, and has become a classic of modern American literature.", 1960, 5, "J.B. Lippincott & Co.", 281);
    Poem* poem1 = new Poem(2, "The Road Not Taken", "A poem about the choices we make in life.", "", "Robert Frost", "Published in 1916, it is one of Frost's most popular works.", 1916, 4, "New York");
    Book* book2 = new Book(3, "1984", "A dystopian social science fiction novel and cautionary tale about the dangers of totalitarianism.", "", "George Orwell", "Published in 1949, the novel is set in a totalitarian society ruled by the Party and its leader, Big Brother.", 1949, 5, "Secker & Warburg", 328);
    Poem* poem2 = new Poem(4, "Ode to a Nightingale", "A poem that explores the themes of nature, transience, and mortality.", "", "John Keats", "Published in 1819, it is one of Keats's most famous works.", 1819, 5, "London");
    Book* book3 = new Book(5, "Pride and Prejudice", "A romantic novel that charts the emotional development of the protagonist, Elizabeth Bennet.", "", "Jane Austen", "Published in 1813, it is a critique of the British landed gentry at the end of the 18th century.", 1813, 5, "T. Egerton, Whitehall", 432);


    AcademicArticle* article1 = new AcademicArticle(6, "Quantum Computing: An Overview", "An overview of the principles and applications of quantum computing.", "", "Alice Smith", "Quantum Computing Journal", "https://example.com/quantum-computing", QDate(2021, 5, 15), 5000, "MIT", 150, true);
    NewspaperArticle* article2 = new NewspaperArticle(7, "Climate Change and Its Impact", "An article discussing the impact of climate change on global weather patterns.", "", "John Doe", "The Daily News", "https://example.com/climate-change", QDate(2022, 3, 10), 1200, "Climate Change: A Global Challenge", true);
    AcademicArticle* article3 = new AcademicArticle(8, "Artificial Intelligence in Healthcare", "Exploring the use of AI in healthcare and its potential benefits.", "", "Emily Johnson", "Healthcare Innovations", "https://example.com/ai-healthcare", QDate(2020, 11, 20), 4500, "Stanford University", 200, true);
    NewspaperArticle* article4 = new NewspaperArticle(9, "Economic Trends in 2023", "An analysis of the economic trends and predictions for the year 2023.", "", "Michael Brown", "Financial Times", "https://example.com/economic-trends-2023", QDate(2023, 1, 5), 1500, "Economic Forecasts", false);

    mediaListController->addMedia(book1);
    mediaListController->addMedia(poem1);
    mediaListController->addMedia(book2);
    mediaListController->addMedia(poem2);
    mediaListController->addMedia(book3);
    mediaListController->addMedia(article1);
    mediaListController->addMedia(article2);
    mediaListController->addMedia(article3);
    mediaListController->addMedia(article4);
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

void MainWindow::updateDeleteButtonState(bool selected) const {
    removeButton->setEnabled(selected);
}

void MainWindow::onRemoveButtonClicked() {
    const QModelIndex currentIndex = listView->currentIndex();
    if (!currentIndex.isValid()) return;

    const Media* media = mediaListController->getMediaList().at(currentIndex.row());
    const QString mediaTitle = media->getTitle();

    const QMessageBox::StandardButton reply = QMessageBox::question(this, "Deleting Media",
                                                              "Do you really want to delete " + mediaTitle + "?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        mediaListController->removeMedia(currentIndex.row());
    }
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

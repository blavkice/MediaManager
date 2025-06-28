#include <QComboBox>
#include <QMessageBox>
#include "MainWindow.h"
#include "Model/LiteratureClasses/Book.h"
#include "Model/LiteratureClasses/Poem.h"
#include "Model/ArticlesClasses/NewspaperArticle.h"
#include "Model/ArticlesClasses/AcademicArticle.h"
#include "View/MediaFilterController.h"
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
    connect(mediaListController, &MediaListController::elementSelected, this, &MainWindow::updateSelectionState);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveButtonClicked);

    // connect the search box to the search function
    connect(searchBox, &QLineEdit::textChanged, this, [this](const QString& text) {
        mediaListController->searchMedia(text);
    });

    // connect the filter box to the filter function
    connect(typeFilterBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
        QVariant data = typeFilterBox->itemData(index);
        if (data.isValid()) {
            const auto filter =
                static_cast<MediaFilterController::MediaTypeFilter>(data.toInt());
            mediaListController->setMediaTypeFilter(filter);
        }
    });

    // json visitor for the import/export actions
    auto jsonVisitor = new JSONVisitor(mediaListController);
    menuBar->setJSONVisitor(jsonVisitor);
    menuBar->importLastDefaultMedia();
}

void MainWindow::initLayouts() {
    hMainViewLayout = new QHBoxLayout(centralWidget);
    // the "multifunctional" widget that will be used to display the media info and edit it
    rightInfoWidget = new RightDynamicWidget(centralWidget);
    // connect the mediaEdit signal in order to clear and re-view
    connect(rightInfoWidget, &RightDynamicWidget::mediaEdited, this, &MainWindow::onMediaEdited);

    // the widget for the left "fixed" part
    auto vLeftWidget = new QWidget(centralWidget);
    // the widget containing the search box and add and remove buttons
    auto utilsWidget = new QWidget(vLeftWidget);
    auto utilsHorizontalLayout = new QHBoxLayout(utilsWidget);
    utilsWidget->setLayout(utilsHorizontalLayout);

    // the filter button
    typeFilterBox = new QComboBox(utilsWidget);
    typeFilterBox->addItem("All", QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::All)));
    typeFilterBox->addItem("Book", QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::Book)));
    typeFilterBox->addItem("Poem", QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::Poem)));
    typeFilterBox->addItem("Academic Article", QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::AcademicArticle)));
    typeFilterBox->addItem("Newspaper Article", QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::NewspaperArticle)));

    vLeftLayout = new QVBoxLayout(vLeftWidget);

    searchBox = new QLineEdit(utilsWidget);
    searchBox->setPlaceholderText("Search by title or short description");
    searchBox->setFixedSize(130, 25);

    typeFilterBox->setFixedSize(60, 25);

    addButton = new QPushButton("+", utilsWidget);
    addButton->setToolTip("add new media");
    addButton->setFixedSize(25, 25);

    removeButton = new QPushButton("-", utilsWidget);
    removeButton->setToolTip("remove selected media");
    removeButton->setFixedSize(25, 25);

    listView = new QListView(vLeftWidget);
    mediaListController = new MediaListController(listView);

    utilsHorizontalLayout->addWidget(typeFilterBox);
    utilsHorizontalLayout->addWidget(searchBox);
    utilsHorizontalLayout->addWidget(addButton);
    utilsHorizontalLayout->addWidget(removeButton);

    vLeftLayout->addWidget(utilsWidget);
    vLeftLayout->addWidget(listView);
    // listView->sIDelegate is already set in MediaListController
    vLeftWidget->setFixedWidth(300);

    hMainViewLayout->addWidget(vLeftWidget);
    hMainViewLayout->addWidget(rightInfoWidget);
    rightInfoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    centralWidget->setLayout(hMainViewLayout);
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

// view the element selected in the list and activate the delete button
void MainWindow::updateSelectionState(bool selected) const {
    if (selected) {
        auto mediaPtr = mediaListController->getCurrentSelectedMedia();
        rightInfoWidget->viewMedia(mediaPtr ? mediaPtr.get() : nullptr);
        removeButton->setEnabled(selected);
    } else {
        rightInfoWidget->clear();
    }
}

void MainWindow::onRemoveButtonClicked() {
    const QModelIndex currentIndex = listView->currentIndex();
    if (!currentIndex.isValid()) return;

    const std::shared_ptr<Media>& mediaPtr = mediaListController->getMediaList().at(currentIndex.row());
    const Media* media = mediaPtr.get();

    const QString mediaTitle = media->getTitle();

    const QMessageBox::StandardButton reply = QMessageBox::question(this, "Deleting Media",
                                                              "Do you really want to delete " + mediaTitle + "?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        mediaListController->removeMedia(currentIndex.row());
        rightInfoWidget->clear();
        searchBox->clear();
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
    // clear search box and selection when adding a new media
    searchBox->clear();
    mediaListController->clearSelection();

    // get type of media and create the widget for it
    if (index < 0) return;
    Media* media = nullptr;
    switch (index) {
        case 0: media = new Book(); break;
        case 1: media = new Poem(); break;
        case 2: media = new AcademicArticle(); break;
        case 3: media = new NewspaperArticle(); break;
    }
    const auto createMediaWidget = new CreateMediaWidget(rightInfoWidget, media);
    try {
        connect(createMediaWidget, &CreateMediaWidget::mediaCreated, this, &MainWindow::onMediaCreated);
        rightInfoWidget->setWidget(createMediaWidget);
    } catch (std::invalid_argument& e) {
        delete createMediaWidget;
        QMessageBox::warning(rightInfoWidget, "Error creating media", e.what());
    }
}

void MainWindow::onMediaCreated(Media* media) const {
    mediaListController->addMedia(std::shared_ptr<Media>(media));
    rightInfoWidget->setMediaCreated();
    // the clear up is done by rightInfoWidget after a widget is set: the previous widget is deleted from heap
    // and so createMediaWidget is deleted correctly everytime
}

void MainWindow::onMediaEdited(Media* media) const {
    mediaListController->populateList();
    rightInfoWidget->viewMedia(media); // "auto-clear" from rightInfoWidget
}

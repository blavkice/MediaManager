#include "MainWindow.h"

#include <QComboBox>
#include <QMessageBox>

#include "Model/AddVisitor.h"
#include "Model/ArticlesClasses/AcademicArticle.h"
#include "Model/ArticlesClasses/NewspaperArticle.h"
#include "Model/LiteratureClasses/Book.h"
#include "Model/LiteratureClasses/Poem.h"
#include "View/CreateMediaWidget.h"
#include "View/MediaFilterController.h"
#include "View/GridView.h"
#include "View/ViewMediaWidget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), menuBar(new MenuBar(this)) {
    resize(800, 600);
    initLayouts();
    setMenuBar(menuBar);
    initAddComboBox();

    // delete button is activated only if a media is selected
    connect(mediaListController, &MediaListController::elementSelected, this, &MainWindow::updateSelectionState);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveButtonClicked);

    // connect the search box to the search function
    connect(searchBox, &QLineEdit::textChanged, this,
            [this](const QString& text) { mediaListController->searchMedia(text); });

    // connect the filter box to the filter function
    connect(typeFilterBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        QVariant data = typeFilterBox->itemData(index);
        if (data.isValid()) {
            const auto filter = static_cast<MediaFilterController::MediaTypeFilter>(data.toInt());
            mediaListController->setMediaTypeFilter(filter);
        }
    });

    // json visitor for the import/export actions
    auto jsonVisitor = new JSONVisitor(mediaListController);
    menuBar->setJSONVisitor(jsonVisitor);
    menuBar->importLastDefaultMedia();
}

void MainWindow::initLayouts() {
    // central stack to switch between views and detailed view
    centralStack = new QStackedWidget(this);

    // TOOLS widget (the top bar with buttons and filters)
    toolsWidget = new QWidget(this);
    auto toolsLayout = new QHBoxLayout(toolsWidget);
    toolsLayout->setContentsMargins(4, 4, 4, 4);

    // the filter button
    typeFilterBox = new QComboBox(toolsWidget);
    typeFilterBox->addItem("All", QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::All)));
    typeFilterBox->addItem("Book", QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::Book)));
    typeFilterBox->addItem("Poem", QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::Poem)));
    typeFilterBox->addItem(
        "Academic Article",
        QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::AcademicArticle)));
    typeFilterBox->addItem(
        "Newspaper Article",
        QVariant::fromValue(static_cast<int>(MediaFilterController::MediaTypeFilter::NewspaperArticle)));
    typeFilterBox->setFixedSize(60, 25);

    searchBox = new QLineEdit(toolsWidget);
    searchBox->setPlaceholderText("Search by title or short description");
    searchBox->setFixedSize(130, 25);

    addButton = new QPushButton("+", toolsWidget);
    addButton->setToolTip("add new media");
    addButton->setFixedSize(25, 25);

    removeButton = new QPushButton("-", toolsWidget);
    removeButton->setToolTip("remove selected media");
    removeButton->setFixedSize(25, 25);

    // switch buttons for the views
    gridViewButton = new QPushButton("Grid View", toolsWidget);
    splitViewButton = new QPushButton("Split View", toolsWidget);
    splitViewButton->setVisible(false);

    // BUTTON STYLING
    gridViewButton->setFixedSize(90, 28);
    splitViewButton->setFixedSize(90, 28);
    typeFilterBox->setFixedSize(90, 28);
    searchBox->setFixedSize(180, 28);
    addButton->setFixedSize(32, 28);
    removeButton->setFixedSize(32, 28);
    addButton->setStyleSheet(
    "QPushButton { background-color: #217a3b; color: white; border-radius: 6px; font-weight: bold; }"
    "QPushButton:hover { background-color: #269944; }"
    );
    removeButton->setStyleSheet(
        "QPushButton { background-color: #a31919; color: white; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #c62d2d; }"
    );
    QFont btnFont = gridViewButton->font();
    btnFont.setBold(true);
    gridViewButton->setFont(btnFont);
    splitViewButton->setFont(btnFont);
    toolsLayout->setSpacing(8);
    toolsLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    toolsLayout->setContentsMargins(8, 6, 8, 6);

    // buttons layout (barra in alto)
    toolsLayout->addWidget(gridViewButton);
    toolsLayout->addWidget(splitViewButton);
    toolsLayout->addWidget(typeFilterBox);
    toolsLayout->addWidget(searchBox);
    toolsLayout->addWidget(addButton);
    toolsLayout->addWidget(removeButton);

    // SPLIT view (default view)
    splitViewWidget = new QWidget(this);
    hMainViewLayout = new QHBoxLayout(splitViewWidget);

    // the widget for the left "fixed" part
    const auto vLeftWidget = new QWidget(splitViewWidget);
    vLeftLayout = new QVBoxLayout(vLeftWidget);

    // the list view for the media elements
    listView = new QListView(vLeftWidget);
    mediaListController = new MediaListController(listView);
    vLeftLayout->addWidget(listView);
    vLeftWidget->setFixedWidth(320);

    hMainViewLayout->addWidget(vLeftWidget);

    // the "multifunctional" widget that will be used to display the media info and edit it
    rightInfoWidget = new RightDynamicWidget(splitViewWidget);
    hMainViewLayout->addWidget(rightInfoWidget);
    rightInfoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    splitViewWidget->setLayout(hMainViewLayout);

    // GRID view (fullscreen view)
    gridView = new GridView(this);

    // STACKED (detailed) view
    centralStack->addWidget(splitViewWidget); // 0: split view (default)
    centralStack->addWidget(gridView);        // 1: grid view fullscreen

    // LAYOUT
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(toolsWidget);     // barra in alto
    mainLayout->addWidget(centralStack);    // viste sotto
    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    // CONNECTIONS
    // to switch to grid view
    connect(gridViewButton, &QPushButton::clicked, this, &MainWindow::switchToGridView);
    // to switch to split view
    connect(splitViewButton, &QPushButton::clicked, this, &MainWindow::switchToSplitView);

    // select an element in the grid view and show it in fullscreen detail
    connect(gridView, &QListView::activated, this, &MainWindow::showFullscreenDetail);
    connect(gridView, &QListView::doubleClicked, this, &MainWindow::showFullscreenDetail);

    // delete button is activated only if a media is selected
    connect(mediaListController, &MediaListController::elementSelected, this, &MainWindow::updateSelectionState);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveButtonClicked);

    // connect the search box to the search function
    connect(searchBox, &QLineEdit::textChanged, this,
            [this](const QString& text) { mediaListController->searchMedia(text); });

    // connect the filter box to the filter function
    connect(typeFilterBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        const QVariant data = typeFilterBox->itemData(index);
        if (data.isValid()) {
            const auto filter = static_cast<MediaFilterController::MediaTypeFilter>(data.toInt());
            mediaListController->setMediaTypeFilter(filter);
        }
    });
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
void MainWindow::updateSelectionState(const bool selected) const {
    if (selected) {
        const auto mediaPtr = mediaListController->getCurrentSelectedMedia();
        rightInfoWidget->viewMedia(mediaPtr ? mediaPtr.get() : nullptr);
        removeButton->setEnabled(selected);
    } else {
        rightInfoWidget->clear();
    }
}

void MainWindow::switchToGridView() {
    gridView->setModel(mediaListController->getFilterModel());
    currentViewMode = FullscreenGrid;
    centralStack->setCurrentWidget(gridView);
    gridViewButton->setVisible(false);
    splitViewButton->setVisible(true);
}

void MainWindow::switchToSplitView() {
    currentViewMode = Split;
    centralStack->setCurrentWidget(splitViewWidget);
    gridViewButton->setVisible(true);
    splitViewButton->setVisible(false);
}

void MainWindow::showFullscreenDetail(const QModelIndex& index) {
    // check if the index is valid
    const auto mediaPtr = index.data(Qt::UserRole).value<std::shared_ptr<Media>>();
    if (!mediaPtr) return;

    // if we are already in fullscreen detail mode, do nothing
    if(detailWidget) {
        centralStack->removeWidget(detailWidget);
        delete detailWidget;
        detailWidget = nullptr;
    }

    detailWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(detailWidget);

    // "back" button to return to the grid view
    QPushButton* backButton = new QPushButton("Back", detailWidget);
    layout->addWidget(backButton, 0, Qt::AlignLeft);

    // detail view for the media
    ViewMediaWidget* viewMediaWidget = new ViewMediaWidget(mediaPtr.get(), detailWidget);
    layout->addWidget(viewMediaWidget);

    // go back to the grid view when the back button is clicked
    connect(backButton, &QPushButton::clicked, this, [this]() {
        centralStack->setCurrentWidget(gridView);
        currentViewMode = FullscreenGrid;
        if(detailWidget) {
            centralStack->removeWidget(detailWidget);
            delete detailWidget;
            detailWidget = nullptr;
        }
    });

    detailWidget->setLayout(layout);
    centralStack->addWidget(detailWidget);
    centralStack->setCurrentWidget(detailWidget);
    currentViewMode = FullscreenDetail;
}

void MainWindow::onRemoveButtonClicked() {
    const QModelIndex currentIndex = listView->currentIndex();
    if (!currentIndex.isValid()) return;

    const std::shared_ptr<Media>& mediaPtr = mediaListController->getMediaList().at(currentIndex.row());
    const Media* media = mediaPtr.get();

    const QString mediaTitle = media->getTitle();

    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Deleting Media", "Do you really want to delete " + mediaTitle + "?", QMessageBox::Yes | QMessageBox::No);
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
        case 0:
            media = new Book();
            break;
        case 1:
            media = new Poem();
            break;
        case 2:
            media = new AcademicArticle();
            break;
        case 3:
            media = new NewspaperArticle();
            break;
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
    rightInfoWidget->viewMedia(media);  // "auto-clear" from rightInfoWidget
}

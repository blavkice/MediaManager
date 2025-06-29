#include "MainWindow.h"

#include <QComboBox>
#include <QMessageBox>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "Model/AddVisitor.h"
#include "Model/ArticlesClasses/AcademicArticle.h"
#include "Model/ArticlesClasses/NewspaperArticle.h"
#include "Model/LiteratureClasses/Book.h"
#include "Model/LiteratureClasses/Poem.h"
#include "View/CreateMediaWidget.h"
#include "View/GridView.h"
#include "View/MediaFilterController.h"
#include "View/ViewMediaWidget.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), menuBar(new MenuBar(this)) {
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
        "QPushButton:hover { background-color: #269944; }");
    removeButton->setStyleSheet(
        "QPushButton { background-color: #a31919; color: white; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #c62d2d; }");
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
    centralStack->addWidget(splitViewWidget);  // 0: split view (default)
    centralStack->addWidget(gridView);         // 1: grid view fullscreen

    // LAYOUT
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(toolsWidget);   // barra in alto
    mainLayout->addWidget(centralStack);  // viste sotto
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

    // edit connection
    connect(rightInfoWidget, &RightDynamicWidget::mediaEdited, this, &MainWindow::onMediaEdited);

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
    // animate the transition to the grid view
    if (centralStack->currentWidget() != gridView)
        animateStackedWidgetTransition(centralStack->currentWidget(), gridView, false);  // false = left->right
    currentViewMode = FullscreenGrid;
    gridViewButton->setVisible(false);
    splitViewButton->setVisible(true);
}

void MainWindow::switchToSplitView() {
    // animate the transition to the split view
    if (centralStack->currentWidget() != splitViewWidget)
        animateStackedWidgetTransition(centralStack->currentWidget(), splitViewWidget, true);  // true = right->left
    currentViewMode = Split;
    gridViewButton->setVisible(true);
    splitViewButton->setVisible(false);
}

void MainWindow::showFullscreenDetail(const QModelIndex& index) {
    // check if the index is valid
    const auto mediaPtr = index.data(Qt::UserRole).value<std::shared_ptr<Media>>();
    if (!mediaPtr) return;

    // if we are already in fullscreen detail mode, do nothing
    if (detailWidget) {
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

    // in order, from fullscreen detail view, to edit the media
    connect(viewMediaWidget, &ViewMediaWidget::mediaEdited, this, &MainWindow::onMediaEdited);

    // go back to the grid view when the back button is clicked
    connect(backButton, &QPushButton::clicked, this, [this]() {
        QWidget* from = centralStack->currentWidget();
        QWidget* to = gridView;
        animateStackedWidgetTransition(from, to, true);

        // clean up after the transition
        QParallelAnimationGroup* group = findChild<QParallelAnimationGroup*>("currentTransitionGroup");
        if (group) {
            connect(group, &QParallelAnimationGroup::finished, this, [this, from]() {
                if (from != gridView && from != splitViewWidget) {
                    centralStack->removeWidget(from);
                    delete from;
                    detailWidget = nullptr;
                }
            });
        }
        currentViewMode = FullscreenGrid;
    });

    detailWidget->setLayout(layout);
    centralStack->addWidget(detailWidget);
    centralStack->setCurrentWidget(detailWidget);
    // animate the transition to the detail view
    animateStackedWidgetTransition(centralStack->currentWidget(), detailWidget, false);
    currentViewMode = FullscreenDetail;
}

void MainWindow::onRemoveButtonClicked() {
    QListView const* activeView = (currentViewMode == FullscreenGrid) ? gridView : listView;
    const QModelIndex currentIndex = activeView->currentIndex();
    if (!currentIndex.isValid()) return;

    // filterController is a MediaFilterController*
    const QModelIndex sourceIndex = mediaListController->getFilterController()->mapToSource(currentIndex);
    if (!sourceIndex.isValid()) return;

    const int actualRow = sourceIndex.row();
    const QList<std::shared_ptr<Media>>& mediaList = mediaListController->getMediaList();
    if (actualRow < 0 || actualRow >= mediaList.size()) return;

    const Media* media = mediaList.at(actualRow).get();
    const QString mediaTitle = media->getTitle();

    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Deleting Media", "Do you really want to delete " + mediaTitle + "?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        mediaListController->removeMedia(actualRow);
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

void MainWindow::onComboBoxActivated(const int index) {
    onMediaSelected(index);
    addComboBox->setVisible(false);
}

// media selected from the combo box for CREATION
void MainWindow::onMediaSelected(const int index) {
    // clear search box and selection when adding a new media
    searchBox->clear();
    mediaListController->clearSelection();

    // get the media type based on the combo box index
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
        default:
            return;
    }

    // grid mode => fullscreen mode for adding new media
    if (currentViewMode == FullscreenGrid) {
        detailWidget = new QWidget(this);
        auto* layout = new QVBoxLayout(detailWidget);

        // back button to go back to grid view without saving
        auto* backButton = new QPushButton("Back", detailWidget);
        layout->addWidget(backButton, 0, Qt::AlignLeft);

        // CreateMediaWidget for adding new media
        auto* createMediaWidget = new CreateMediaWidget(detailWidget, media);
        layout->addWidget(createMediaWidget);

        // handle "mediaCreated" signal: add the new media, close fullscreen, return to grid view
        connect(createMediaWidget, &CreateMediaWidget::mediaCreated, this, [this](Media* newMedia) {
            onMediaCreated(newMedia);
            // close fullscreen and return to grid view
            centralStack->setCurrentWidget(gridView);
            if (detailWidget) {
                centralStack->removeWidget(detailWidget);
                delete detailWidget;
                detailWidget = nullptr;
            }
            currentViewMode = FullscreenGrid;
        });

        // handle "back" button: just return to grid view
        connect(backButton, &QPushButton::clicked, this, [this]() {
            centralStack->setCurrentWidget(gridView);
            if (detailWidget) {
                centralStack->removeWidget(detailWidget);
                delete detailWidget;
                detailWidget = nullptr;
            }
            currentViewMode = FullscreenGrid;
        });

        detailWidget->setLayout(layout);
        centralStack->addWidget(detailWidget);
        centralStack->setCurrentWidget(detailWidget);
        currentViewMode = FullscreenDetail;
    } else {
        // list or split view => right info widget for adding new media
        const auto createMediaWidget = new CreateMediaWidget(rightInfoWidget, media);
        try {
            connect(createMediaWidget, &CreateMediaWidget::mediaCreated, this, &MainWindow::onMediaCreated);
            rightInfoWidget->setWidget(createMediaWidget);
        } catch (std::invalid_argument& e) {
            delete createMediaWidget;
            QMessageBox::warning(rightInfoWidget, "Error creating media", e.what());
        }
    }
}

void MainWindow::onMediaCreated(Media* media) const {
    mediaListController->addMedia(std::shared_ptr<Media>(media));
    rightInfoWidget->setMediaCreated();
    // the clear up is done by rightInfoWidget after a widget is set: the previous widget is deleted from heap
    // and so createMediaWidget is deleted correctly everytime
}

void MainWindow::onMediaEdited(Media* media) {
    // update the media in the list
    mediaListController->populateList();
    listView->setModel(mediaListController->getFilterModel());
    gridView->setModel(mediaListController->getFilterModel());

    // if in fullscreen detail mode, return to grid view
    if (currentViewMode == FullscreenDetail) {
        // Return to grid view and clean up detailWidget
        centralStack->setCurrentWidget(gridView);
        if (detailWidget) {
            centralStack->removeWidget(detailWidget);
            delete detailWidget;
            detailWidget = nullptr;
        }
        currentViewMode = FullscreenGrid;
    } else {
        rightInfoWidget->viewMedia(media);  // update the right panel
    }

    gridView->reset();  // reload everything from the model
    gridView->viewport()->update();
}

void MainWindow::animateStackedWidgetTransition(QWidget* from, QWidget* to, bool leftToRight) {
    const int w = centralStack->width();

    // start position
    const QRect endGeom = centralStack->geometry();
    QRect startGeom = endGeom;
    startGeom.moveLeft(leftToRight ? -w : w);

    // place to offscreen
    to->setGeometry(startGeom);
    centralStack->setCurrentWidget(to);

    // from out to in
    const auto animOut = new QPropertyAnimation(from, "geometry");
    animOut->setDuration(80);
    animOut->setStartValue(endGeom);
    animOut->setEndValue(leftToRight ? endGeom.translated(w, 0) : endGeom.translated(-w, 0));

    const auto animIn = new QPropertyAnimation(to, "geometry");
    animIn->setDuration(80);
    animIn->setStartValue(startGeom);
    animIn->setEndValue(endGeom);

    const auto group = new QParallelAnimationGroup(this);
    group->addAnimation(animOut);
    group->addAnimation(animIn);

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

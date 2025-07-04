#include "MediaListController.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QImage>
#include <QScrollBar>

#include "InfoListPainter.h"

MediaListController::MediaListController(QListView* listView, QObject* parent) : QObject(parent), listView(listView) {
    // populate the list view
    model = new QStandardItemModel(listView);
    if (listView == nullptr) {
        throw std::invalid_argument("listView cannot be null");
    }

    // initialize the filter controller
    filterController = new MediaFilterController(this);
    filterController->setSourceModel(model);
    listView->setModel(filterController);

    // important attributes for listview
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    // listView->setModel(model); the model is already set in the filter controller
    listView->setItemDelegate(new InfoListPainter(listView));
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listView->verticalScrollBar()->setSingleStep(10);

    // connect the selectionChanged signal to the onSelectionChanged slot
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &MediaListController::onSelectionChanged);
}

void MediaListController::onSelectionChanged(const QItemSelection& selected) {
    emit elementSelected(!selected.isEmpty());
}

void MediaListController::addMedia(const std::shared_ptr<Media>& media) {
    QSignalBlocker blocker(listView->selectionModel());
    mediaList.append(media);
    populateList();
}

void MediaListController::clearMedia() {
    mediaList.clear();
    model->clear();
}

QAbstractItemModel* MediaListController::getFilterModel() const {
    return filterController;
}

MediaFilterController* MediaListController::getFilterController() const {
    return filterController;
}

void MediaListController::populateList() const {
    model->clear();
    for (const auto& media : mediaList) {
        const auto item = new QStandardItem();

        // check if the image is valid and load it into the item
        const QString appDirPath = QCoreApplication::applicationDirPath();
        QDir dir(appDirPath);
        const QString imgPath = dir.filePath("media") + "/" + media->getId() + ".jpg";
        QImage image(imgPath);
        if (image.isNull()) {
            image.load(":default.jpg");
            qDebug() << "image is null";
        }

        item->setData(QVariant::fromValue(image), Qt::DecorationRole);
        item->setData(media->getTitle(), Qt::DisplayRole);
        item->setData(media->getShortDescription(), Qt::UserRole + 1);
        item->setData(QVariant::fromValue(media), Qt::UserRole);  // to filter type of media
        model->appendRow(item);
    }
}

void MediaListController::removeMedia(const int index) {
    if (index < 0 || index >= mediaList.size()) return;
    QModelIndex sourceIndex = filterController->mapToSource(listView->model()->index(index, 0));

    if (!sourceIndex.isValid()) return;

    int actualRow = sourceIndex.row();

    QSignalBlocker blocker(listView->selectionModel());
    mediaList.removeAt(actualRow);
    populateList();
    searchMedia("");

    listView->clearSelection();
    listView->setCurrentIndex(QModelIndex());
}

void MediaListController::searchMedia(const QString& searchText) const {
    filterController->setSearchQuery(searchText);
    listView->clearSelection();
    listView->setCurrentIndex(QModelIndex());
}

QList<std::shared_ptr<Media>> MediaListController::getMediaList() const {
    return mediaList;
}

std::shared_ptr<Media> MediaListController::getCurrentSelectedMedia() const {
    const QModelIndex currentIndex = listView->currentIndex();
    if (!currentIndex.isValid()) {
        return nullptr;
    }

    // due to filtering the index might be off so we need to map it to the source model
    const QModelIndex sourceIndex = filterController->mapToSource(currentIndex);
    if (!sourceIndex.isValid() || sourceIndex.row() >= mediaList.size()) {
        return nullptr;
    }

    return mediaList.at(sourceIndex.row());
}

void MediaListController::setMediaList(const QList<Media*>& mediaList) {
    this->mediaList.clear();
    for (Media* media : mediaList) {
        this->mediaList.append(std::shared_ptr<Media>(media));
    }
    populateList();
}

void MediaListController::clearSelection() const {
    listView->clearSelection();
    listView->setCurrentIndex(QModelIndex());
}

void MediaListController::setMediaTypeFilter(const MediaFilterController::MediaTypeFilter filter) const {
    filterController->setMediaTypeFilter(filter);
}

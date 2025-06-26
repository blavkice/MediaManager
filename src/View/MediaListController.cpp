#include "MediaListController.h"
#include "InfoListPainter.h"
#include <QApplication>
#include <QImage>
#include <QDir>
#include <QScrollBar>
#include <QCoreApplication>

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
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
        this, &MediaListController::onSelectionChanged);

    // important attributes for listview
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    // listView->setModel(model); the model is already set in the filter controller
    listView->setItemDelegate(new InfoListPainter(listView));
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listView->verticalScrollBar()->setSingleStep(10);

    // connect the selectionChanged signal to the onSelectionChanged slot
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MediaListController::onSelectionChanged);
}

void MediaListController::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
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

void MediaListController::populateList() const {
    model->clear();
    for(const auto& media : mediaList) {
        const auto item = new QStandardItem();

        // check if the image is valid and load it into the item
        const QString appDirPath = QCoreApplication::applicationDirPath();
        QDir dir(appDirPath);
        dir.cdUp(); dir.cdUp();
        const QString imgPath = dir.filePath("media") + "/" + media->getId() + ".jpg";
        qDebug() << "imgPath:" << imgPath;

        QImage image(imgPath);
        if (image.isNull()) {
            image.load(":default.jpg");
            qDebug() << "image is null";
        }

        item->setData(QVariant::fromValue(image), Qt::DecorationRole);
        item->setData(media->getTitle(), Qt::DisplayRole);
        item->setData(media->getShortDescription(), Qt::UserRole + 1);
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
    // TBD: those 2 lines, should they be HERE?, if different views are implements?
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
    return mediaList.at(currentIndex.row());
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

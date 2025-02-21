#include "MediaListController.h"
#include "InfoListPainter.h"
#include <QApplication>
#include <QImage>
#include <QDir>
#include <QScrollBar>
#include <QCoreApplication>

MediaListController::MediaListController(QListView* listView, QObject* parent) : listView(listView), QObject(parent) {
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


void MediaListController::addMedia(Media* media) {
    // block signals to avoid unnecessary selection (stability...)
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

    // get the index in the filtered list
    QModelIndex sourceIndex = filterController->mapToSource(listView->model()->index(index, 0));
    if (!sourceIndex.isValid()) return;

    // get the actual row in the original list
    int actualRow = sourceIndex.row();

    // block signals to avoid unnecessary selection (stability...)
    QSignalBlocker blocker(listView->selectionModel());

    // delete the element from the original list
    delete mediaList.takeAt(actualRow);

    // repopulate the list and reapply the search
    populateList();
    searchMedia("");

    // clear selection
    listView->clearSelection();
    listView->setCurrentIndex(QModelIndex());
}

void MediaListController::searchMedia(const QString& searchText) const {
    filterController->setSearchQuery(searchText);
    // TBD: those 2 lines, should they be HERE?, if different views are implementes?
    listView->clearSelection();
    listView->setCurrentIndex(QModelIndex());
}

QList<Media*> MediaListController::getMediaList() const {
    return mediaList;
}

Media* MediaListController::getCurrentSelectedMedia() const {
    const QModelIndex currentIndex = listView->currentIndex();
    if (!currentIndex.isValid()) {
        return nullptr;
    }
    // map the filtered index to the original model index
    const QModelIndex sourceIndex = filterController->mapToSource(currentIndex);
    if (!sourceIndex.isValid() || sourceIndex.row() >= mediaList.size()) {
        return nullptr;
    }
    return mediaList.at(sourceIndex.row());
}

void MediaListController::setMediaList(const QList<Media*>& mediaList) {
    this->mediaList = mediaList;
    populateList();
}

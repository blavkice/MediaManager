#include "MediaListController.h"
#include "InfoListPainter.h"
#include <QApplication>
#include <QImage>

MediaListController::MediaListController(QListView* listView, QObject* parent) : listView(listView), QObject(parent) {
    // connect the selectionChanged signal to the onSelectionChanged slot
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MediaListController::onSelectionChanged);

    // populate the list view
    model = new QStandardItemModel(listView);
    if (listView == nullptr) {
        throw std::invalid_argument("listView cannot be null");
    }
    listView->setModel(model);
    listView->setItemDelegate(new InfoListPainter(listView));
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listView->setStyleSheet("QListView::item:selected { background-color: blue; }");
}

void MediaListController::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
    emit elementSelected(!selected.isEmpty());
}

void MediaListController::addMedia(Media* media) {
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
        QImage image(media->getImagePath());
        if (image.isNull()) {
            image.load(":default.jpg");
        }
        item->setData(QVariant::fromValue(image), Qt::DecorationRole);
        item->setData(media->getTitle(), Qt::DisplayRole);
        item->setData(media->getShortDescription(), Qt::UserRole + 1);
        model->appendRow(item);
    }
}

void MediaListController::removeMedia(const int index) {
    if (index < 0 || index >= mediaList.size()) return;
    delete mediaList.takeAt(index);
    listView->model()->removeRow(index);
    populateList();
}

QList<Media*> MediaListController::getMediaList() const {
    return mediaList;
}

void MediaListController::setMediaList(const QList<Media*>& mediaList) {
    this->mediaList = mediaList;
    populateList();
}

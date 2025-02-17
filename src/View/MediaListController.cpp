#include "MediaListController.h"
#include "InfoListPainter.h"
#include <QApplication>
#include <QImage>

MediaListController::MediaListController(QListView* listView) : listView(listView) {
    model = new QStandardItemModel(listView);
    if (listView == nullptr) {
        throw std::invalid_argument("listView cannot be null");
    }
    listView->setModel(model);
    listView->setItemDelegate(new InfoListPainter(listView));
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void MediaListController::addMedia(Media* media) {
    mediaList.push_back(media);
    populateList();
}

void MediaListController::populateList() const {
    model->clear();
    for(const auto& media : mediaList) {
        QStandardItem* item = new QStandardItem();
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

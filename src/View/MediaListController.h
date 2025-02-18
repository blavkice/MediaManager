#ifndef MEDIALISTCONTROLLER_H
#define MEDIALISTCONTROLLER_H
#include <QListView>
#include <QStandardItemModel>
#include <QList>
#include "../Model/Media.h"

class MediaListController {
private:
    QListView* listView;
    QStandardItemModel* model;
    QList<Media*> mediaList;
    void populateList() const;
public:
    explicit MediaListController(QListView* listView);
    void addMedia(Media* media);
    void clearMedia();
    QList<Media*> getMediaList() const;
    void setMediaList(const QList<Media*>& mediaList);
};

#endif //MEDIALISTCONTROLLER_H

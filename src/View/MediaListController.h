#ifndef MEDIALISTCONTROLLER_H
#define MEDIALISTCONTROLLER_H
#include <QListView>
#include <QStandardItemModel>
#include <vector>
#include "../Model/Media.h"

using std::vector;

class MediaListController {
private:
    QListView* listView;
    QStandardItemModel* model;
    vector<Media*> mediaList;
    void populateList() const;
public:
    explicit MediaListController(QListView* listView);
    void addMedia(Media* media);
};

#endif //MEDIALISTCONTROLLER_H

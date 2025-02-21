#ifndef MEDIALISTCONTROLLER_H
#define MEDIALISTCONTROLLER_H
#include <QListView>
#include <QStandardItemModel>
#include <QList>
#include "MediaFilterController.h"
#include "../Model/Media.h"

class MediaListController : public QObject {
    Q_OBJECT
private:
    QListView* listView;
    QStandardItemModel* model;
    MediaFilterController* filterController;
    QList<Media*> mediaList;
public:
    explicit MediaListController(QListView* listView, QObject* parent = nullptr);
    void addMedia(Media* media);
    void clearMedia();
    QList<Media*> getMediaList() const;
    void populateList() const;
    void setMediaList(const QList<Media*>& mediaList);
    void removeMedia(int index);
    void searchMedia(const QString& searchText) const;
    Media* getCurrentSelectedMedia() const;
signals:
    void elementSelected(bool selected);
private slots:
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
};

#endif //MEDIALISTCONTROLLER_H

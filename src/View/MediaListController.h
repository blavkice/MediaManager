#ifndef MEDIALISTCONTROLLER_H
#define MEDIALISTCONTROLLER_H
#include <QList>
#include <QListView>
#include <QStandardItemModel>

#include "../Model/Media.h"
#include "MediaFilterController.h"

class MediaListController : public QObject {
    Q_OBJECT
   private:
    QListView* listView;
    QStandardItemModel* model;
    MediaFilterController* filterController;
    QList<std::shared_ptr<Media>> mediaList;

   public:
    explicit MediaListController(QListView* listView, QObject* parent = nullptr);
    void addMedia(const std::shared_ptr<Media>& media);
    void clearMedia();
    void populateList() const;
    void setMediaList(const QList<Media*>& mediaList);
    void removeMedia(int index);
    void searchMedia(const QString& searchText) const;
    QList<std::shared_ptr<Media>> getMediaList() const;
    std::shared_ptr<Media> getCurrentSelectedMedia() const;
    QAbstractItemModel* getFilterModel() const;
    void setMediaTypeFilter(MediaFilterController::MediaTypeFilter filter) const;
    void clearSelection() const;
   signals:
    void elementSelected(bool selected);
   private slots:
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
};

#endif  // MEDIALISTCONTROLLER_H

#ifndef MEDIAFILTERCONTROLLER_H
#define MEDIAFILTERCONTROLLER_H
#include <QSortFilterProxyModel>
#include <QString>

#include "../Model/Media.h"

class MediaFilterController : public QSortFilterProxyModel {
    Q_OBJECT
   public:
    explicit MediaFilterController(QObject* parent = nullptr);
    void setSearchQuery(const QString& searchQuery);
    enum class MediaTypeFilter { All, Book, Poem, AcademicArticle, NewspaperArticle };
    void setMediaTypeFilter(MediaTypeFilter type);
   private:
    QString searchQuery;
    MediaTypeFilter mediaTypeFilter = MediaTypeFilter::All;  // default filter is set to all
   protected:
    // in the base class it is marked protected so we leave it as such
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
};

#endif  // MEDIAFILTERCONTROLLER_H

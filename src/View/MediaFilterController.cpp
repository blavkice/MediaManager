#include "MediaFilterController.h"
#include "MediaFilterController.h"
#include "../Model/LiteratureClasses/Book.h"
#include "../Model/LiteratureClasses/Poem.h"
#include "../Model/ArticlesClasses/AcademicArticle.h"
#include "../Model/ArticlesClasses/NewspaperArticle.h"

MediaFilterController::MediaFilterController(QObject* parent):
    QSortFilterProxyModel(parent) {
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(Qt::DisplayRole);
}

void MediaFilterController:: setSearchQuery(const QString& searchQuery) {
    this->searchQuery = searchQuery;
    // and trigger the filtering
    invalidateFilter();
}

bool MediaFilterController::filterAcceptsRow(const int sourceRow, const QModelIndex& sourceParent) const {
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    // text search filter
    const QString mediaName = sourceModel()->data(index, Qt::DisplayRole).toString();
    if (!mediaName.contains(searchQuery, Qt::CaseInsensitive))
        return false;

    // "type" filter
    auto mediaPtr = sourceModel()->data(index, Qt::UserRole).value<std::shared_ptr<Media>>();
    if (!mediaPtr)
        return false;

    // if no type filter is set, accept all media types
    if (mediaTypeFilter == MediaTypeFilter::All)
        return true;

    // otherwise filter by type
    switch (mediaTypeFilter) {
        case MediaTypeFilter::Book:
            return dynamic_cast<Book*>(mediaPtr.get()) != nullptr;
        case MediaTypeFilter::Poem:
            return dynamic_cast<Poem*>(mediaPtr.get()) != nullptr;
        case MediaTypeFilter::AcademicArticle:
            return dynamic_cast<AcademicArticle*>(mediaPtr.get()) != nullptr;
        case MediaTypeFilter::NewspaperArticle:
            return dynamic_cast<NewspaperArticle*>(mediaPtr.get()) != nullptr;
        default:
            return true;
    }
}

void MediaFilterController::setMediaTypeFilter(const MediaTypeFilter type) {
    mediaTypeFilter = type;
    invalidateFilter();
}

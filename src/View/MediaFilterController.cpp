#include "MediaFilterController.h"

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
    const QString mediaName = sourceModel()->data(index, Qt::DisplayRole).toString();
    return mediaName.contains(searchQuery, Qt::CaseInsensitive);
}

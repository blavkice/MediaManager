#ifndef MEDIAFILTERCONTROLLER_H
#define MEDIAFILTERCONTROLLER_H
#include <QSortFilterProxyModel>
#include <QString>
#include "../Model/Media.h"

class MediaFilterController : public QSortFilterProxyModel {
    Q_OBJECT
private:
    QString searchQuery;
    // TDB other filters
protected:
    // in the base class it is marked protected so we leave it as such
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
public:
    explicit MediaFilterController(QObject* parent = nullptr);
    void setSearchQuery(const QString& searchQuery);
    // TBD other filters
};

#endif //MEDIAFILTERCONTROLLER_H

#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QListView>
#include <QStyledItemDelegate>
#include <QImage>
#include <QPainter>

// delegate to render items in a grid view
class GridViewDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit GridViewDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const override;
};

class GridView : public QListView {
    Q_OBJECT
public:
    explicit GridView(QWidget* parent = nullptr) : QListView(parent) {
        setItemDelegate(new GridViewDelegate(this));
        setViewMode(QListView::IconMode);
        setResizeMode(QListView::Adjust);
        setMovement(QListView::Static);
        setSpacing(10);
        setUniformItemSizes(true);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setGridSize(QSize(150, 180));
    }
};

#endif //GRIDVIEW_H

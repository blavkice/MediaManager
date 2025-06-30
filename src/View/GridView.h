#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QListView>
#include <QStyledItemDelegate>

class GridView : public QListView {
    Q_OBJECT
   public:
    explicit GridView(QWidget* parent = nullptr);

    // custom delegate for rendering items
    class GridViewDelegate : public QStyledItemDelegate {
       public:
        explicit GridViewDelegate(QObject* parent = nullptr);
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    };
};

#endif  // GRIDVIEW_H

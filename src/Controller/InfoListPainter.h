#ifndef INFOLISTPAINTER_H
#define INFOLISTPAINTER_H
#include <QStyledItemDelegate>
#include <QPainter>

class InfoListPainter : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit InfoListPainter(QObject* parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif //INFOLISTPAINTER_H

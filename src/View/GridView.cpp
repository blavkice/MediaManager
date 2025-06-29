#include "GridView.h"

void GridViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    // draw the specific item in the grid view
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    const QRect rect = option.rect.adjusted(5, 5, -5, -5);
    const QImage img = index.data(Qt::DecorationRole).value<QImage>();
    const QString title = index.data(Qt::DisplayRole).toString();

    // selection highlight
    if (option.state & QStyle::State_Selected)
        painter->fillRect(rect, option.palette.highlight());

    const QRect imgRect(rect.x(), rect.y(), rect.width(), rect.height() - 30);
    painter->drawImage(imgRect, img.scaled(imgRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    const QRect textRect(rect.x(), rect.bottom() - 25, rect.width(), 20);
    painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, title);
    painter->restore();
}

QSize GridViewDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const {
    return QSize(150, 180);
}

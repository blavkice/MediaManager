#include "GridView.h"

void GridViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    // draw the specific item in the grid view
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    const QRect rect = option.rect.adjusted(10, 10, -10, -10);
    const QImage img = index.data(Qt::DecorationRole).value<QImage>();
    const QString title = index.data(Qt::DisplayRole).toString();

    // selection highlight
    if (option.state & QStyle::State_Selected) painter->fillRect(rect, option.palette.highlight());

    // draw the image and title
    const int imgHeight = rect.height() - 48;  // more space for text
    const QRect imgRect(rect.x(), rect.y(), rect.width(), imgHeight);
    painter->drawImage(imgRect, img.scaled(imgRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    const QRect textRect(rect.x(), rect.y() + imgHeight + 6, rect.width(), rect.height() - imgHeight - 8);

    // good wrapping, big font for more visibility
    QFont font = painter->font();
    font.setPointSize(font.pointSize() + 2);
    painter->setFont(font);

    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);
    opt.setAlignment(Qt::AlignCenter);

    painter->drawText(textRect, title, opt);

    painter->restore();
}

QSize GridViewDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const {
    return QSize(180, 230);
}

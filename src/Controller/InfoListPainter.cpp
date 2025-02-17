#include "InfoListPainter.h"
//#include <QApplication>
#include <QImage>

InfoListPainter::InfoListPainter(QObject* parent) : QStyledItemDelegate(parent) { }

void InfoListPainter::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->save();

    QRect rect = option.rect;
    QImage img = index.data(Qt::DecorationRole).value<QImage>();
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 1).toString();

    QRect imgRect = QRect(rect.left(), rect.top(), 50, 50);
    QRect titleRect = QRect(rect.left() + 60, rect.top(), rect.width() - 60, 20);
    QRect descriptionRect = QRect(rect.left() + 60, rect.top() + 20, rect.width() - 60, rect.height() - 20);

    painter->drawImage(imgRect, img);
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, title);

    QTextOption textOption;
    // to wrap the short description text and avoid it to be cut
    textOption.setWrapMode(QTextOption::WordWrap);
    painter->drawText(descriptionRect, description, textOption);

    painter->restore();
}

QSize InfoListPainter::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QFontMetrics fontMetrics(option.font);
    QString description = index.data(Qt::UserRole + 1).toString();
    int textHeight = fontMetrics.boundingRect(QRect(0, 0, option.rect.width() - 60, 0), Qt::TextWordWrap, description).height();
    return QSize(option.rect.width(), 50 + textHeight);
}
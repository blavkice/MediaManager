#include "InfoListPainter.h"
//#include <QApplication>
#include <QImage>
#include <QPainterPath>

InfoListPainter::InfoListPainter(QObject* parent) : QStyledItemDelegate(parent) { }

void InfoListPainter::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->save();

    QRect rect = option.rect;
    QImage img = index.data(Qt::DecorationRole).value<QImage>();
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 1).toString();

    auto imgRect = QRect(rect.left(), rect.top(), 50, 50);
    auto titleRect = QRect(rect.left() + 60, rect.top(), rect.width() - 80, 20);
    auto descriptionRect = QRect(rect.left() + 60, rect.top() + 20, rect.width() - 80, rect.height() - 20);

    // draw image (rounded)
    QPainterPath path;
    path.addRoundedRect(imgRect, 25, 25);
    painter->setClipPath(path);
    painter->drawImage(imgRect, img);
    painter->setClipping(false);

    // draw title
    QFont titleFont = painter->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2); // Increase font size by 2 points
    painter->setFont(titleFont);
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, title);

    // resetting the font back to normal
    QFont descriptionFont = painter->font();
    descriptionFont.setBold(false);
    descriptionFont.setPointSize(descriptionFont.pointSize() - 2); // Reset font size
    painter->setFont(descriptionFont);

    // draw the description and avoid text overflow
    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WordWrap); // Enable word wrap
    painter->drawText(descriptionRect, description, textOption);

    painter->restore();
}

QSize InfoListPainter::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QFontMetrics fontMetrics(option.font);
    QString description = index.data(Qt::UserRole + 1).toString();
    int textHeight = fontMetrics.boundingRect(QRect(0, 0, option.rect.width() - 60, 0), Qt::TextWordWrap, description).height();
    return QSize(option.rect.width(), 70 + textHeight);
}
#include "GridView.h"

#include <QFontDatabase>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>
#include <QTextOption>

// custom delegate for "modern" look
GridView::GridViewDelegate::GridViewDelegate(QObject* parent) : QStyledItemDelegate(parent) {
}

void GridView::GridViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                       const QModelIndex& index) const {
    // draw the specific item in the grid view
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    const QRect rect = option.rect.adjusted(10, 10, -10, -10);
    const QImage img = index.data(Qt::DecorationRole).value<QImage>();
    const QString title = index.data(Qt::DisplayRole).toString();

    // selection highlight
    if (option.state & QStyle::State_Selected) painter->fillRect(rect, QColor("#217a3b"));  // green soft

    // draw "card" background with rounded corners
    QRectF cardRect = rect;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 18));  // soft shadow
    painter->drawRoundedRect(cardRect.adjusted(2, 6, 8, 10), 18, 18);

    painter->setBrush(QColor("#232323"));  // dark background
    painter->drawRoundedRect(cardRect, 18, 18);

    // draw the image with rounded corners (a lil bit)
    const int imgHeight = rect.height() - 52;
    QRect imgRect(rect.x() + 12, rect.y() + 12, rect.width() - 24, imgHeight - 8);

    QPainterPath clipPath;
    clipPath.addRoundedRect(imgRect, 14, 14);
    painter->setClipPath(clipPath);
    painter->drawImage(imgRect, img.scaled(imgRect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    painter->setClipping(false);

    // custom title with font
    QRect textRect(rect.x() + 8, imgRect.bottom() + 12, rect.width() - 16, rect.height() - imgHeight - 16);

    static int aceFontId = QFontDatabase::addApplicationFont(":/fonts/AceSansExtraBold.ttf");
    static QString aceFontFamily = QFontDatabase::applicationFontFamilies(aceFontId).isEmpty()
                                       ? QString()
                                       : QFontDatabase::applicationFontFamilies(aceFontId).at(0);
    QFont font(aceFontFamily, 13, QFont::Bold);
    painter->setFont(font);

    painter->setPen(Qt::white);
    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);
    opt.setAlignment(Qt::AlignCenter);

    // ellipsize the title if it is too long
    QString elidedTitle = painter->fontMetrics().elidedText(title, Qt::ElideRight, textRect.width() * 1);
    painter->drawText(textRect, elidedTitle, opt);

    painter->restore();
}

QSize GridView::GridViewDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const {
    return QSize(190, 250);
}

GridView::GridView(QWidget* parent) : QListView(parent) {
    setItemDelegate(new GridViewDelegate(this));
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Adjust);
    setMovement(QListView::Static);
    setSpacing(16);
    setUniformItemSizes(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setGridSize(QSize(190, 250));
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("margin: 0; padding: 0;");
    setViewportMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

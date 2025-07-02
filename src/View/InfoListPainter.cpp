#include "InfoListPainter.h"

#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QImage>
#include <QPainterPath>
#include <QTextOption>

InfoListPainter::InfoListPainter(QObject* parent) : QStyledItemDelegate(parent) {
}

void InfoListPainter::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->save();

    // antialiasing lol
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    // rectangle with a little bit of padding
    QRect rect = option.rect.adjusted(10, 10, -10, -10);
    QImage img = index.data(Qt::DecorationRole).value<QImage>();
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 1).toString();

    if (option.state & QStyle::State_Selected) {
        QPainterPath selectionPath;
        selectionPath.addRoundedRect(rect, 12, 12);
        painter->fillPath(selectionPath, option.palette.highlight());
        painter->setPen(option.palette.highlightedText().color());
    } else {
        painter->setPen(option.palette.text().color());
    }

    // calculate title height
    int titleHeight = 25;  // default height
    QFont titleFont;
    const int titleFontId = QFontDatabase::addApplicationFont(":/Fonts/AceSansExtrabold.ttf");
    if (titleFontId != -1) {
        QString titleFontFamily = QFontDatabase::applicationFontFamilies(titleFontId).at(0);
        titleFont = QFont(titleFontFamily);
        titleFont.setPointSize(12);
        titleFont.setBold(true);

        // calculate title height based on the font and text
        QFontMetrics titleMetrics(titleFont);
        QRect titleBounds = titleMetrics.boundingRect(QRect(0, 0, rect.width() - 85, 0), Qt::TextWordWrap, title);
        titleHeight = qMax(25, titleBounds.height() + 5);  // minimum 25px, with 5px padding
    }

    auto imgRect = QRect(rect.left(), rect.top() + 5, 60, 60);
    auto titleRect = QRect(rect.left() + 75, rect.top() + 5, rect.width() - 85, titleHeight);
    auto descriptionRect =
        QRect(rect.left() + 75, rect.top() + 10 + titleHeight, rect.width() - 85, rect.height() - titleHeight - 15);

    // draw rounded image with shadow effect
    QPainterPath imgPath;
    imgPath.addRoundedRect(imgRect, 30, 30);
    painter->setClipPath(imgPath);
    painter->drawImage(imgRect, img);
    painter->setClipping(false);

    if (titleFontId != -1) {
        painter->setFont(titleFont);

        // wrapping
        QTextOption titleOption;
        titleOption.setWrapMode(QTextOption::WordWrap);
        titleOption.setAlignment(Qt::AlignLeft | Qt::AlignTop);
        painter->drawText(titleRect, title, titleOption);
    }

    // reset font to default font for description
    QFont descFont = option.font;
    descFont.setPointSize(descFont.pointSize() + 1);
    painter->setFont(descFont);
    painter->setPen(option.palette.text().color());

    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WordWrap);
    painter->drawText(descriptionRect, description, textOption);

    painter->restore();
}

QSize InfoListPainter::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QFontMetrics fontMetrics(option.font);
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 1).toString();

    // calculate title height
    int titleHeight = 25;  // default height
    const int titleFontId = QFontDatabase::addApplicationFont(":/Fonts/AceSansExtrabold.ttf");
    if (titleFontId != -1) {
        QString titleFontFamily = QFontDatabase::applicationFontFamilies(titleFontId).at(0);
        QFont titleFont(titleFontFamily);
        titleFont.setPointSize(12);
        titleFont.setBold(true);

        QFontMetrics titleMetrics(titleFont);
        QRect titleBounds =
            titleMetrics.boundingRect(QRect(0, 0, option.rect.width() - 85, 0), Qt::TextWordWrap, title);
        titleHeight = qMax(25, titleBounds.height() + 5);
    }

    // calculate description height
    int textHeight =
        fontMetrics.boundingRect(QRect(0, 0, option.rect.width() - 85, 0), Qt::TextWordWrap, description).height();

    return QSize(option.rect.width(), 80 + titleHeight + textHeight);
}
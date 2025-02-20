#include <QUuid>
#include "Media.h"

Media::Media() :
    id(QUuid::createUuid().toString(QUuid::WithoutBraces)),
    title("Unknown"), shortDescription(" "),
    imagePath(":/img/default.jpg") { }

Media::Media(const QString &title, const QString &short_description, const QString &image_path)
    : title(title),
      shortDescription(short_description),
      imagePath(image_path) {
    id = QUuid::createUuid().toString(QUuid::WithoutBraces);
}

Media::~Media() = default;

void Media::accept(Visitor *visitor) {
    visitor->visit(this);
}

QString Media::getId() const {
    return id;
}

QString Media::getTitle() const {
    return title;
}

QString Media::getShortDescription() const {
    return shortDescription;
}

QString Media::getImagePath() const {
    return imagePath;
}

void Media::setId(const QString &id) {
    this->id = id;
}

void Media::setTitle(const QString &title) {
    this->title = title;
}

void Media::setShortDescription(const QString &short_description) {
    this->shortDescription = short_description;
}

void Media::setImagePath(const QString &image_path) {
    this->imagePath = image_path;
}

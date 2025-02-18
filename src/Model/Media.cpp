#include "Media.h"

Media::Media() : id(0), title("Unknown"), shortDescription(" "), imagePath(":/img/default.jpg") { }

Media::Media(const unsigned int id, const QString &title, const QString &short_description, const QString &image_path)
    : id(id),
      title(title),
      shortDescription(short_description),
      imagePath(image_path) { }

Media::~Media() = default;

void Media::accept(Visitor *visitor) {
    visitor->visit(this);
}

unsigned int Media::getId() const {
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

void Media::setId(const unsigned int id) {
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

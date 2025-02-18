#include "Literature.h"

Literature::Literature(unsigned int id, const QString &title, const QString &short_description, const QString &image_path,
    const QString &author, const QString &long_description, int publication_year, short rating)
    : Media(id, title, short_description, image_path),
      author(author),
      longDescription(long_description),
      publicationYear(publication_year),
      rating(rating) { }

Literature::~Literature() = default;

void Literature::accept(Visitor *visitor) {
    visitor->visit(this);
}

QString Literature::getAuthor() const {
    return author;
}

QString Literature::getLongDescription() const {
    return longDescription;
}

int Literature::getPublicationYear() const {
    return publicationYear;
}

short Literature::getRating() const {
    return rating;
}

void Literature::setAuthor(const QString &author) {
    this->author = author;
}

void Literature::setLongDescription(const QString &long_description) {
    this->longDescription = long_description;
}

void Literature::setPublicationYear(const int publication_year) {
    this->publicationYear = publication_year;
}

void Literature::setRating(const short rating) {
    this->rating = rating;
}

#include "Poem.h"

Poem::Poem() : city(" ") {
}

Poem::Poem(const QString& title, const QString& short_description, const QString& image_path, const QString& author,
           const QString& long_description, int publication_year, short rating, const QString& city)
    : Literature(title, short_description, image_path, author, long_description, publication_year, rating), city(city) {
}

void Poem::accept(Visitor* visitor) {
    visitor->visit(this);
}

QString Poem::getCity() const {
    return city;
}

void Poem::setCity(const QString& city) {
    this->city = city;
}

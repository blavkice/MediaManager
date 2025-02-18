#include "Book.h"

Book::Book(const unsigned int id, const QString &title, const QString &short_description, const QString &image_path,
    const QString &author, const QString &long_description, const int publication_year, const short rating,
    const QString &publishing, const unsigned int pages)
    : Literature(id, title, short_description, image_path, author, long_description, publication_year, rating),
      publishing(publishing),
      pages(pages) { }

void Book::accept(Visitor *visitor) {
    visitor->visit(this);
}

QString Book::getPublishing() const {
    return publishing;
}

unsigned int Book::getPages() const {
    return pages;
}

void Book::setPublishing(const QString &publishing) {
    this->publishing = publishing;
}

void Book::setPages(const unsigned int pages) {
    this->pages = pages;
}

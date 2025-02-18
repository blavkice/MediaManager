#include "Articles.h"

Articles::Articles() : author("No author set"), category("General"), url("No url"), date(QDate()), wordCount(0) { }

Articles::Articles(const unsigned int id, const QString &title, const QString &short_description, const QString &image_path,
    const QString &author, const QString &category, const QString &url, const QDate &date, const unsigned int word_count)
    : Media(id, title, short_description, image_path),
      author(author),
      category(category),
      url(url),
      date(date),
      wordCount(word_count) { }

Articles::~Articles() = default;

void Articles::accept(Visitor *visitor) {
    visitor->visit(this);
}

QString Articles::getAuthor() const {
    return author;
}

QString Articles::getCategory() const {
    return category;
}

QString Articles::getUrl() const {
    return url;
}

QDate Articles::getDate() const {
    return date;
}

unsigned int Articles::getWordCount() const {
    return wordCount;
}

void Articles::setAuthor(const QString &author) {
    this->author = author;
}

void Articles::setCategory(const QString &category) {
    this->category = category;
}

void Articles::setUrl(const QString &url) {
    this->url = url;
}

void Articles::setDate(const QDate &date) {
    this->date = date;
}

void Articles::setWordCount(const unsigned int word_count) {
    wordCount = word_count;
}

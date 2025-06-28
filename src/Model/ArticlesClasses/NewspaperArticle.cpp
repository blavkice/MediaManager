#include "NewspaperArticle.h"

NewspaperArticle::NewspaperArticle(const QString &title, const QString &short_description, const QString &image_path,
                                   const QString &author, const QString &category, const QString &url,
                                   const QDate &date, const unsigned int word_count, const QString &headline,
                                   const bool politics)
    : Articles(title, short_description, image_path, author, category, url, date, word_count),
      headline(headline),
      politics(politics) {
}

void NewspaperArticle::accept(Visitor *visitor) {
    visitor->visit(this);
}

QString NewspaperArticle::getHeadline() const {
    return headline;
}

bool NewspaperArticle::isPolitics() const {
    return politics;
}

void NewspaperArticle::setHeadline(const QString &headline) {
    this->headline = headline;
}

void NewspaperArticle::setPolitics(const bool politics) {
    this->politics = politics;
}

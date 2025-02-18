#ifndef ARTICLES_H
#define ARTICLES_H
#include <QDate>
#include "Media.h"

class Articles : public Media {
private:
    QString author;
    QString category;
    QString url;
    QDate date;
    unsigned int wordCount;
public:
    Articles() = default;
    Articles(const unsigned int id, const QString &title, const QString &short_description, const QString &image_path,
        const QString &author, const QString &category, const QString &url, const QDate &date, const unsigned int word_count);
    ~Articles() override = 0; // to make the class abstract
    QString getAuthor() const;
    QString getCategory() const;
    QString getUrl() const;
    QDate getDate() const;
    unsigned int getWordCount() const;
    void setAuthor(const QString &author);
    void setCategory(const QString &category);
    void setUrl(const QString &url);
    void setDate(const QDate &date);
    void setWordCount(unsigned int word_count);
    virtual void accept(Visitor *visitor) override = 0;
};

#endif //ARTICLES_H

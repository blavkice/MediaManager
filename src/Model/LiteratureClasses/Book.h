#ifndef BOOK_H
#define BOOK_H
#include "../Literature.h"

class Book : public Literature {
private:
    QString publishing;
    unsigned int pages;
public:
    Book(unsigned int id, const QString &title, const QString &short_description, const QString &image_path,
        const QString &author, const QString &long_description, int publication_year, short rating,
        const QString &publishing, unsigned int pages);
    QString getPublishing() const;
    unsigned int getPages() const;
    void setPublishing(const QString &publishing);
    void setPages(unsigned int pages);
};

#endif //BOOK_H

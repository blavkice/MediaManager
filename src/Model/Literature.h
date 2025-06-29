#ifndef LITERATURE_H
#define LITERATURE_H
#include <QString>

#include "Media.h"

class Literature : public Media {
   private:
    QString author;
    QString longDescription;
    int publicationYear;
    short rating;

   public:
    Literature();
    Literature(const QString &title, const QString &short_description, const QString &image_path, const QString &author,
               const QString &long_description, int publication_year, short rating);
    virtual ~Literature() = 0;  // to make the class abstract
    QString getAuthor() const;
    QString getLongDescription() const;
    int getPublicationYear() const;
    short getRating() const;
    void setAuthor(const QString &author);
    void setLongDescription(const QString &long_description);
    void setPublicationYear(int publication_year);
    void setRating(short rating);
    virtual void accept(Visitor *visitor) override = 0;
};

#endif  // LITERATURE_H

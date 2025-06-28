#ifndef NEWSPAPERARTICLE_H
#define NEWSPAPERARTICLE_H
#include "../Articles.h"

class NewspaperArticle : public Articles {
   private:
    QString headline;
    bool politics;

   public:
    NewspaperArticle() = default;
    NewspaperArticle(const QString &title, const QString &short_description, const QString &image_path,
                     const QString &author, const QString &category, const QString &url, const QDate &date,
                     const unsigned int word_count, const QString &headline, const bool politics);
    QString getHeadline() const;
    bool isPolitics() const;
    void setHeadline(const QString &headline);
    void setPolitics(const bool politics);
    void accept(Visitor *visitor) override;
};

#endif  // NEWSPAPERARTICLE_H

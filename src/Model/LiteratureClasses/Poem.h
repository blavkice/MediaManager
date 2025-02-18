#ifndef POEM_H
#define POEM_H
#include "../Literature.h"

class Poem : public Literature {
private:
    QString city;
public:
    Poem();
    Poem(unsigned int id, const QString &title, const QString &short_description, const QString &image_path,
        const QString &author, const QString &long_description, int publication_year, short rating,
        const QString &city);
    QString getCity() const;
    void setCity(const QString &city);
    void accept(Visitor *visitor) override;
};

#endif //POEM_H

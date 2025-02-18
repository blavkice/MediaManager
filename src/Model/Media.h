#ifndef MEDIA_H
#define MEDIA_H
#include "Visitable.h"
#include "Visitor.h"
#include <QString>

class Media : public Visitable {
private:
    unsigned int id;
    QString title;
    QString shortDescription;
    QString imagePath;
public:
    Media() = default;
    Media(unsigned int id, const QString &title, const QString &short_description, const QString &image_path = ":/img/default.jpg");
    virtual ~Media() = 0; // to make the class abstract
    unsigned int getId() const;
    QString getTitle() const;
    QString getShortDescription() const;
    QString getImagePath() const;
    void setId(unsigned int id);
    void setTitle(const QString &title);
    void setShortDescription(const QString &short_description);
    void setImagePath(const QString &image_path);
    // methods for the design patterns
    void accept(Visitor *visitor) override;
};

#endif //MEDIA_H

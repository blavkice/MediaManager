#ifndef MEDIA_H
#define MEDIA_H
#include "Visitable.h"
#include "Visitor.h"
#include <QString>

class Media : public Visitable {
private:
    QString id;
    QString title;
    QString shortDescription;
    QString imagePath;
public:
    Media();
    Media(const QString &title, const QString &short_description, const QString &image_path = ":/img/default.jpg");
    virtual ~Media() = 0; // to make the class abstract
    QString getId() const;
    QString getTitle() const;
    QString getShortDescription() const;
    QString getImagePath() const;
    // note: setId is to be used only on imports!
    void setId(const QString &id);
    void setTitle(const QString &title);
    void setShortDescription(const QString &short_description);
    void setImagePath(const QString &image_path);
    // methods for the design patterns
    virtual void accept(Visitor *visitor) override = 0;
};

// to allow the use of "std::shared_ptr<Media>" in qt signals and slots
// specifically, in order to use it in the search filter inside MediaListController
Q_DECLARE_METATYPE(std::shared_ptr<Media>);

#endif //MEDIA_H

#include "JSONVisitor.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "ArticlesClasses/AcademicArticle.h"
#include "ArticlesClasses/NewspaperArticle.h"
#include "LiteratureClasses/Book.h"
#include "LiteratureClasses/Poem.h"

JSONVisitor::JSONVisitor(MediaListController* mediaListController) : mediaListController(mediaListController) {
}

// qDebugs are left for debugging purposes, they can be obv removed
bool JSONVisitor::importFromFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file";
        return false;
    }

    const QByteArray data = file.readAll();
    const QJsonDocument doc(QJsonDocument::fromJson(data));
    if (doc.isNull()) {
        qDebug() << "Failed to create JSON doc.";
        return false;
    }
    if (!doc.isArray()) {
        qDebug() << "JSON is not an array.";
        return false;
    }

    QJsonArray jsonArray = doc.array();

    mediaListController->clearMedia();
    for (const QJsonValue& value : jsonArray) {
        if (!value.isObject()) {
            qDebug() << "JSON value is not an object.";
            continue;
        }
        QJsonObject jsonObject = value.toObject();
        Media* media = deserialize(jsonObject);
        if (media) {
            mediaListController->addMedia(std::shared_ptr<Media>(media));
        } else {
            qDebug() << "Failed to deserialize media.";
        }
    }
    mediaListController->populateList();
    return true;
}

bool JSONVisitor::exportToFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    jsonArray = QJsonArray();
    for (const std::shared_ptr<Media>& mediaPtr : mediaListController->getMediaList()) {
        if (mediaPtr) {
            mediaPtr->accept(this);
        }
    }

    // clean up the images directory: if an image is not used, it will be removed
    QDir dir(QCoreApplication::applicationDirPath());
    QDir mediaDir(dir.filePath("media"));

    // collect all used image file names (normalized to lower-case)
    QSet<QString> usedImages;
    for (const auto& media : mediaListController->getMediaList()) {
        QString fileName = QFileInfo(media->getImagePath()).fileName().toLower();
        if (!fileName.isEmpty()) {
            usedImages.insert(fileName);
        }
    }

    // get all image files in the media directory (normalized to lower-case and by extension)
    QStringList allFiles = mediaDir.entryList(QStringList() << "*.jpg" << "*.jpeg", QDir::Files);
    for (const QString& fileName : allFiles) {
        QString normalized = fileName.toLower();
        if (!usedImages.contains(normalized)) {
            mediaDir.remove(fileName);
        }
    }

    const QJsonDocument doc(jsonArray);
    qDebug() << "Writing JSON data to file:" << doc.toJson();
    file.write(doc.toJson());

    return true;
}

void JSONVisitor::visit(Media* media) {
    QJsonObject json;
    json["id"] = media->getId();
    json["title"] = media->getTitle();
    json["shortDescription"] = media->getShortDescription();
    json["imagePath"] = media->getImagePath();
    jsonArray.append(json);
}

void JSONVisitor::visit(Literature* literature) {
    QJsonObject json;
    json["id"] = literature->getId();
    json["title"] = literature->getTitle();
    json["shortDescription"] = literature->getShortDescription();
    json["imagePath"] = literature->getImagePath();
    json["author"] = literature->getAuthor();
    json["longDescription"] = literature->getLongDescription();
    json["publicationYear"] = literature->getPublicationYear();
    json["rating"] = literature->getRating();
    jsonArray.append(json);
}

void JSONVisitor::visit(Articles* article) {
    QJsonObject json;
    json["id"] = article->getId();
    json["title"] = article->getTitle();
    json["shortDescription"] = article->getShortDescription();
    json["imagePath"] = article->getImagePath();
    json["author"] = article->getAuthor();
    json["category"] = article->getCategory();
    json["url"] = article->getUrl();
    json["date"] = article->getDate().toString(Qt::ISODate);
    json["wordCount"] = static_cast<int>(article->getWordCount());
    jsonArray.append(json);
}

void JSONVisitor::visit(Book* book) {
    QJsonObject json;
    json["id"] = book->getId();
    json["title"] = book->getTitle();
    json["shortDescription"] = book->getShortDescription();
    json["imagePath"] = book->getImagePath();
    json["author"] = book->getAuthor();
    json["longDescription"] = book->getLongDescription();
    json["publicationYear"] = book->getPublicationYear();
    json["rating"] = book->getRating();
    json["publishing"] = book->getPublishing();
    json["pages"] = static_cast<int>(book->getPages());
    json["type"] = "Book";
    jsonArray.append(json);
}

void JSONVisitor::visit(Poem* poem) {
    QJsonObject json;
    json["id"] = poem->getId();
    json["title"] = poem->getTitle();
    json["shortDescription"] = poem->getShortDescription();
    json["imagePath"] = poem->getImagePath();
    json["author"] = poem->getAuthor();
    json["longDescription"] = poem->getLongDescription();
    json["publicationYear"] = poem->getPublicationYear();
    json["rating"] = poem->getRating();
    json["city"] = poem->getCity();
    json["type"] = "Poem";
    jsonArray.append(json);
}

void JSONVisitor::visit(AcademicArticle* academicArticle) {
    QJsonObject json;
    json["id"] = academicArticle->getId();
    json["title"] = academicArticle->getTitle();
    json["shortDescription"] = academicArticle->getShortDescription();
    json["imagePath"] = academicArticle->getImagePath();
    json["author"] = academicArticle->getAuthor();
    json["category"] = academicArticle->getCategory();
    json["url"] = academicArticle->getUrl();
    json["date"] = academicArticle->getDate().toString(Qt::ISODate);
    json["wordCount"] = static_cast<int>(academicArticle->getWordCount());
    json["universityName"] = academicArticle->getUniversityName();
    json["citationCount"] = static_cast<int>(academicArticle->getCitationCount());
    json["peerReviewed"] = academicArticle->isPeerReviewed();
    json["type"] = "AcademicArticle";
    jsonArray.append(json);
}

void JSONVisitor::visit(NewspaperArticle* newspaperArticle) {
    QJsonObject json;
    json["id"] = newspaperArticle->getId();
    json["title"] = newspaperArticle->getTitle();
    json["shortDescription"] = newspaperArticle->getShortDescription();
    json["imagePath"] = newspaperArticle->getImagePath();
    json["author"] = newspaperArticle->getAuthor();
    json["category"] = newspaperArticle->getCategory();
    json["url"] = newspaperArticle->getUrl();
    json["date"] = newspaperArticle->getDate().toString(Qt::ISODate);
    json["wordCount"] = static_cast<int>(newspaperArticle->getWordCount());
    json["headline"] = newspaperArticle->getHeadline();
    json["politics"] = newspaperArticle->isPolitics();
    json["type"] = "NewspaperArticle";
    jsonArray.append(json);
}

Media* JSONVisitor::deserialize(const QJsonObject& json) {
    const QString type = json["type"].toString();
    Media* media = nullptr;

    if (type == "Book") {
        media = new Book();
        static_cast<Book*>(media)->setPublishing(json["publishing"].toString());
        static_cast<Book*>(media)->setPages(json["pages"].toInt());
    } else if (type == "Poem") {
        media = new Poem();
        static_cast<Poem*>(media)->setCity(json["city"].toString());
    } else if (type == "AcademicArticle") {
        media = new AcademicArticle();
        static_cast<AcademicArticle*>(media)->setUniversityName(json["universityName"].toString());
        static_cast<AcademicArticle*>(media)->setCitationCount(json["citationCount"].toInt());
        static_cast<AcademicArticle*>(media)->setPeerReviewed(json["peerReviewed"].toBool());
    } else if (type == "NewspaperArticle") {
        media = new NewspaperArticle();
        static_cast<NewspaperArticle*>(media)->setHeadline(json["headline"].toString());
        static_cast<NewspaperArticle*>(media)->setPolitics(json["politics"].toBool());
    }

    if (media) {
        media->setId(json["id"].toString());
        media->setTitle(json["title"].toString());
        media->setShortDescription(json["shortDescription"].toString());
        media->setImagePath(json["imagePath"].toString());

        if (type == "Book" || type == "Poem") {
            Literature* literature = static_cast<Literature*>(media);
            literature->setAuthor(json["author"].toString());
            literature->setLongDescription(json["longDescription"].toString());
            literature->setPublicationYear(json["publicationYear"].toInt());
            literature->setRating(json["rating"].toInt());
        } else if (type == "AcademicArticle" || type == "NewspaperArticle") {
            Articles* article = static_cast<Articles*>(media);
            article->setAuthor(json["author"].toString());
            article->setCategory(json["category"].toString());
            article->setUrl(json["url"].toString());
            article->setDate(QDate::fromString(json["date"].toString(), Qt::ISODate));
            article->setWordCount(json["wordCount"].toInt());
        }
    }

    return media;
}

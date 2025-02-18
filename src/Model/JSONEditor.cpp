#include "JSONEditor.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "LiteratureClasses/Book.h"
#include "LiteratureClasses/Poem.h"
#include "ArticlesClasses/AcademicArticle.h"
#include "ArticlesClasses/NewspaperArticle.h"

JSONEditor::JSONEditor(MediaListController* mediaListController)
    : mediaListController(mediaListController) { }

bool JSONEditor::importFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray jsonArray = doc.array();

    mediaListController->clearMedia();
    for (const QJsonValue& value : jsonArray) {
        QJsonObject jsonObject = value.toObject();
        Media* media = jsonToMedia(jsonObject);
        if (media) {
            mediaListController->addMedia(media);
        }
    }
    // now we have to refresh the list view ui
    mediaListController->populateList();
    return true;
}

bool JSONEditor::exportToFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    QJsonArray jsonArray;
    for (const Media* media : mediaListController->getMediaList()) {
        QJsonObject jsonObject;
        mediaToJson(media, jsonObject);
        jsonArray.append(jsonObject);
    }

    QJsonDocument doc(jsonArray);
    qDebug() << "Writing JSON data to file:" << doc.toJson();
    file.write(doc.toJson());
    return true;
}

void JSONEditor::mediaToJson(const Media* media, QJsonObject& json) {
    json["title"] = media->getTitle();
    json["shortDescription"] = media->getShortDescription();
    json["imagePath"] = media->getImagePath();

    if (const Literature* literature = dynamic_cast<const Literature*>(media)) {
        json["author"] = literature->getAuthor();
        json["longDescription"] = literature->getLongDescription();
        json["publicationYear"] = literature->getPublicationYear();
        json["rating"] = literature->getRating();

        if (const Book* book = dynamic_cast<const Book*>(literature)) {
            json["type"] = "Book";
            json["publishing"] = book->getPublishing();
            json["pages"] = static_cast<int>(book->getPages());
        } else if (const Poem* poem = dynamic_cast<const Poem*>(literature)) {
            json["type"] = "Poem";
            json["city"] = poem->getCity();
        }
    } else if (const Articles* article = dynamic_cast<const Articles*>(media)) {
        json["author"] = article->getAuthor();
        json["category"] = article->getCategory();
        json["url"] = article->getUrl();
        json["date"] = article->getDate().toString(Qt::ISODate);
        json["wordCount"] = static_cast<int>(article->getWordCount());

        if (const AcademicArticle* academicArticle = dynamic_cast<const AcademicArticle*>(article)) {
            json["type"] = "AcademicArticle";
            json["universityName"] = academicArticle->getUniversityName();
            json["citationCount"] = static_cast<int>(academicArticle->getCitationCount());
            json["peerReviewed"] = academicArticle->isPeerReviewed();
        } else if (const NewspaperArticle* newspaperArticle = dynamic_cast<const NewspaperArticle*>(article)) {
            json["type"] = "NewspaperArticle";
            json["headline"] = newspaperArticle->getHeadline();
            json["politics"] = newspaperArticle->isPolitics();
        }
    }
}

Media* JSONEditor::jsonToMedia(const QJsonObject& json) {
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
        media->setTitle(json["title"].toString());
        media->setShortDescription(json["shortDescription"].toString());
        media->setImagePath(json["imagePath"].toString());

        if (Literature* literature = dynamic_cast<Literature*>(media)) {
            literature->setAuthor(json["author"].toString());
            literature->setLongDescription(json["longDescription"].toString());
            literature->setPublicationYear(json["publicationYear"].toInt());
            literature->setRating(json["rating"].toInt());
        } else if (Articles* article = dynamic_cast<Articles*>(media)) {
            article->setAuthor(json["author"].toString());
            article->setCategory(json["category"].toString());
            article->setUrl(json["url"].toString());
            article->setDate(QDate::fromString(json["date"].toString(), Qt::ISODate));
            article->setWordCount(json["wordCount"].toInt());
        }
    }
    return media;
}

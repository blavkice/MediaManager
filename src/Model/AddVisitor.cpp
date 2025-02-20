#include <QLabel>
#include <QLineEdit>
#include <QImage>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include "AddVisitor.h"

#include <QPushButton>

AddVisitor::AddVisitor(QLayout* layout, QWidget* parent) : layout(layout) { }

void AddVisitor::visit(Media* media) {
    layout->addWidget(new QLabel("Title:"));
    auto titleEdit = new QLineEdit(media->getTitle());
    layout->addWidget(titleEdit);
    inputFields["title"] = titleEdit;

    layout->addWidget(new QLabel("Short Description:"));
    auto shortDescriptionEdit = new QLineEdit(media->getShortDescription());
    layout->addWidget(shortDescriptionEdit);
    inputFields["shortDescription"] = shortDescriptionEdit;

    // image selection is managed by createMediaWidget
}

void AddVisitor::visit(Literature* literature) {
    visit(static_cast<Media*>(literature));
    layout->addWidget(new QLabel("Author:"));
    auto authorEdit = new QLineEdit(literature->getAuthor());
    layout->addWidget(authorEdit);
    inputFields["author"] = authorEdit;

    layout->addWidget(new QLabel("Long Description:"));
    auto longDescriptionEdit = new QLineEdit(literature->getLongDescription());
    layout->addWidget(longDescriptionEdit);
    inputFields["longDescription"] = longDescriptionEdit;

    layout->addWidget(new QLabel("Publication Year:"));
    auto publicationYearEdit = new QLineEdit(QString::number(literature->getPublicationYear()));
    layout->addWidget(publicationYearEdit);
    inputFields["publicationYear"] = publicationYearEdit;

    layout->addWidget(new QLabel("Rating:"));
    auto ratingEdit = new QLineEdit(QString::number(literature->getRating()));
    layout->addWidget(ratingEdit);
    inputFields["rating"] = ratingEdit;
}

void AddVisitor::visit(Articles* article) {
    visit(static_cast<Media*>(article));
    layout->addWidget(new QLabel("Author:"));
    auto authorEdit = new QLineEdit(article->getAuthor());
    layout->addWidget(authorEdit);
    inputFields["author"] = authorEdit;

    layout->addWidget(new QLabel("Category:"));
    auto categoryEdit = new QLineEdit(article->getCategory());
    layout->addWidget(categoryEdit);
    inputFields["category"] = categoryEdit;

    layout->addWidget(new QLabel("URL:"));
    auto urlEdit = new QLineEdit(article->getUrl());
    layout->addWidget(urlEdit);
    inputFields["url"] = urlEdit;

    layout->addWidget(new QLabel("Date:"));
    auto dateEdit = new QLineEdit(article->getDate().toString(Qt::ISODate));
    layout->addWidget(dateEdit);
    inputFields["date"] = dateEdit;

    layout->addWidget(new QLabel("Word Count:"));
    auto wordCountEdit = new QLineEdit(QString::number(article->getWordCount()));
    layout->addWidget(wordCountEdit);
    inputFields["wordCount"] = wordCountEdit;
}

void AddVisitor::visit(Book* book) {
    visit(static_cast<Literature*>(book));
    layout->addWidget(new QLabel("Publishing:"));
    auto publishingEdit = new QLineEdit(book->getPublishing());
    layout->addWidget(publishingEdit);
    inputFields["publishing"] = publishingEdit;

    layout->addWidget(new QLabel("Pages:"));
    auto pagesEdit = new QLineEdit(QString::number(book->getPages()));
    layout->addWidget(pagesEdit);
    inputFields["pages"] = pagesEdit;
}

void AddVisitor::visit(Poem* poem) {
    visit(static_cast<Literature*>(poem));
    layout->addWidget(new QLabel("City:"));
    auto cityEdit = new QLineEdit(poem->getCity());
    layout->addWidget(cityEdit);
    inputFields["city"] = cityEdit;
}

void AddVisitor::visit(AcademicArticle* academicArticle) {
    visit(static_cast<Articles*>(academicArticle));
    layout->addWidget(new QLabel("University Name:"));
    auto universityNameEdit = new QLineEdit(academicArticle->getUniversityName());
    layout->addWidget(universityNameEdit);
    inputFields["universityName"] = universityNameEdit;

    layout->addWidget(new QLabel("Citation Count:"));
    auto citationCountEdit = new QLineEdit(QString::number(academicArticle->getCitationCount()));
    layout->addWidget(citationCountEdit);
    inputFields["citationCount"] = citationCountEdit;

    layout->addWidget(new QLabel("Peer Reviewed:"));
    auto peerReviewedEdit = new QLineEdit(academicArticle->isPeerReviewed() ? "Yes" : "No");
    layout->addWidget(peerReviewedEdit);
    inputFields["peerReviewed"] = peerReviewedEdit;
}

void AddVisitor::visit(NewspaperArticle* newspaperArticle) {
    visit(static_cast<Articles*>(newspaperArticle));
    layout->addWidget(new QLabel("Headline:"));
    auto headlineEdit = new QLineEdit(newspaperArticle->getHeadline());
    layout->addWidget(headlineEdit);
    inputFields["headline"] = headlineEdit;

    layout->addWidget(new QLabel("Politics:"));
    auto politicsEdit = new QLineEdit(newspaperArticle->isPolitics() ? "Yes" : "No");
    layout->addWidget(politicsEdit);
    inputFields["politics"] = politicsEdit;
}

void AddVisitor::saveInput(Media* media) {
    media->setTitle(inputFields["title"]->text());
    media->setShortDescription(inputFields["shortDescription"]->text());

    // now for the image there is a separate method checking its integrity and saving it into ./media/
    saveImage(media, media->getImagePath());

    // do for literature
    if (auto literature = dynamic_cast<Literature*>(media)) {
        literature->setAuthor(inputFields["author"]->text());
        literature->setLongDescription(inputFields["longDescription"]->text());
        literature->setPublicationYear(inputFields["publicationYear"]->text().toInt());
        literature->setRating(inputFields["rating"]->text().toInt());
        if (auto book = dynamic_cast<Book*>(media)) {
            book->setPublishing(inputFields["publishing"]->text());
            book->setPages(inputFields["pages"]->text().toInt());
        } else {
            auto poem = static_cast<Poem*>(media);
            poem->setCity(inputFields["city"]->text());
        }
        // now articles
    } else { auto article = dynamic_cast<Articles*>(media);
        article->setAuthor(inputFields["author"]->text());
        article->setCategory(inputFields["category"]->text());
        article->setUrl(inputFields["url"]->text());
        article->setDate(QDate::fromString(inputFields["date"]->text(), Qt::ISODate));
        article->setWordCount(inputFields["wordCount"]->text().toInt());
        if (auto academicArticle = dynamic_cast<AcademicArticle*>(media)) {
            academicArticle->setUniversityName(inputFields["universityName"]->text());
            academicArticle->setCitationCount(inputFields["citationCount"]->text().toInt());
            academicArticle->setPeerReviewed(inputFields["peerReviewed"]->text() == "Yes");
        } else {
            auto newspaperArticle = static_cast<NewspaperArticle*>(media);
            newspaperArticle->setHeadline(inputFields["headline"]->text());
            newspaperArticle->setPolitics(inputFields["politics"]->text() == "Yes");
        }
    }
}

void AddVisitor::saveImage(Media* media, const QString& imagePath) {
    qDebug() << "media uuid:" << media->getId();
    if (imagePath.isEmpty()) {
        media->setImagePath(":default.jpg");
    }
    QImage image(imagePath);
    if (image.isNull()) {
        qDebug() << "Error: Invalid image at path:" << imagePath;
        media->setImagePath(":default.jpg");
        return;
    }

    // then the image is valid
    const QString appDirPath = QCoreApplication::applicationDirPath();
    QDir dir(appDirPath);
    dir.cdUp(); dir.cdUp();

    // check existence of media directory
    const QString mediaDirPath = dir.filePath("media");
    if (!dir.exists(mediaDirPath)) {
        dir.mkpath(mediaDirPath);
    }

    // TBD, saveFilePath is maybe redundant, we can just access the media folder and search for id.jpg
    const QString newFilePath = mediaDirPath + "/" + media->getId() + ".jpg";

    if (image.save(newFilePath, "JPG", 75)) {
        media->setImagePath(media->getId() + ".jpg");
        qDebug() << "Image saved to:" << newFilePath;
    } else media->setImagePath(":default.jpg");
}

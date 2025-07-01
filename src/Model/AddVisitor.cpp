#include "AddVisitor.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QDateEdit>
#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

AddVisitor::AddVisitor(QLayout* layout) : layout(layout) {
}

void AddVisitor::visit(Media* media) {
    layout->addWidget(new QLabel("Title:"));
    auto titleEdit = new QLineEdit(media->getTitle());
    layout->addWidget(titleEdit);
    inputFields["title"] = titleEdit;

    layout->addWidget(new QLabel("Short Description:"));
    auto shortDescriptionEdit = new QLineEdit(media->getShortDescription());
    layout->addWidget(shortDescriptionEdit);
    inputFields["shortDescription"] = shortDescriptionEdit;

    // all image selection is managed by createMediaWidget!
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
    auto publicationYearSpin = new QSpinBox();
    publicationYearSpin->setRange(1500, QDate::currentDate().year());
    publicationYearSpin->setValue(literature->getPublicationYear());
    layout->addWidget(publicationYearSpin);
    inputFields["publicationYear"] = publicationYearSpin;

    layout->addWidget(new QLabel("Rating:"));
    auto ratingSpin = new QDoubleSpinBox();
    ratingSpin->setRange(0, 5);
    ratingSpin->setDecimals(1);
    ratingSpin->setValue(literature->getRating());
    layout->addWidget(ratingSpin);
    inputFields["rating"] = ratingSpin;
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
    auto dateEdit = new QDateEdit();
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(article->getDate().isValid() ? article->getDate() : QDate::currentDate());
    layout->addWidget(dateEdit);
    inputFields["date"] = dateEdit;

    layout->addWidget(new QLabel("Word Count:"));
    auto wordCountSpin = new QSpinBox();
    wordCountSpin->setRange(0, 100000);
    wordCountSpin->setValue(article->getWordCount());
    layout->addWidget(wordCountSpin);
    inputFields["wordCount"] = wordCountSpin;
}

void AddVisitor::visit(Book* book) {
    visit(static_cast<Literature*>(book));
    layout->addWidget(new QLabel("Publishing:"));
    auto publishingEdit = new QLineEdit(book->getPublishing());
    layout->addWidget(publishingEdit);
    inputFields["publishing"] = publishingEdit;

    layout->addWidget(new QLabel("Pages:"));
    auto pagesSpin = new QSpinBox();
    pagesSpin->setRange(1, 10000);
    pagesSpin->setValue(book->getPages());
    layout->addWidget(pagesSpin);
    inputFields["pages"] = pagesSpin;
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
    auto citationCountSpin = new QSpinBox();
    citationCountSpin->setRange(0, 100000);
    citationCountSpin->setValue(academicArticle->getCitationCount());
    layout->addWidget(citationCountSpin);
    inputFields["citationCount"] = citationCountSpin;

    layout->addWidget(new QLabel("Peer Reviewed:"));
    auto peerReviewedCombo = new QComboBox();
    peerReviewedCombo->addItems({"No", "Yes"});
    peerReviewedCombo->setCurrentIndex(academicArticle->isPeerReviewed() ? 1 : 0);
    layout->addWidget(peerReviewedCombo);
    inputFields["peerReviewed"] = peerReviewedCombo;
}

void AddVisitor::visit(NewspaperArticle* newspaperArticle) {
    visit(static_cast<Articles*>(newspaperArticle));
    layout->addWidget(new QLabel("Headline:"));
    auto headlineEdit = new QLineEdit(newspaperArticle->getHeadline());
    layout->addWidget(headlineEdit);
    inputFields["headline"] = headlineEdit;

    layout->addWidget(new QLabel("Politics:"));
    auto politicsCombo = new QComboBox();
    politicsCombo->addItems({"No", "Yes"});
    politicsCombo->setCurrentIndex(newspaperArticle->isPolitics() ? 1 : 0);
    layout->addWidget(politicsCombo);
    inputFields["politics"] = politicsCombo;
}

bool AddVisitor::saveInput(Media* media) {
    // campi base
    auto titleEdit = qobject_cast<QLineEdit*>(inputFields["title"]);
    media->setTitle(titleEdit ? titleEdit->text() : "");

    auto shortDescEdit = qobject_cast<QLineEdit*>(inputFields["shortDescription"]);
    media->setShortDescription(shortDescEdit ? shortDescEdit->text() : "");

    // LITERATURE
    if (auto literature = dynamic_cast<Literature*>(media)) {
        auto authorEdit = qobject_cast<QLineEdit*>(inputFields["author"]);
        literature->setAuthor(authorEdit ? authorEdit->text() : "");

        auto longDescEdit = qobject_cast<QLineEdit*>(inputFields["longDescription"]);
        literature->setLongDescription(longDescEdit ? longDescEdit->text() : "");

        auto yearSpin = qobject_cast<QSpinBox*>(inputFields["publicationYear"]);
        literature->setPublicationYear(yearSpin ? yearSpin->value() : 0);

        auto ratingSpin = qobject_cast<QDoubleSpinBox*>(inputFields["rating"]);
        literature->setRating(ratingSpin ? ratingSpin->value() : 0);

        if (auto book = dynamic_cast<Book*>(media)) {
            auto publishingEdit = qobject_cast<QLineEdit*>(inputFields["publishing"]);
            book->setPublishing(publishingEdit ? publishingEdit->text() : "");

            auto pagesSpin = qobject_cast<QSpinBox*>(inputFields["pages"]);
            book->setPages(pagesSpin ? pagesSpin->value() : 0);
        } else {
            auto poem = static_cast<Poem*>(media);
            auto cityEdit = qobject_cast<QLineEdit*>(inputFields["city"]);
            poem->setCity(cityEdit ? cityEdit->text() : "");
        }
    }
    // ARTICLES
    else {
        auto article = dynamic_cast<Articles*>(media);
        auto authorEdit = qobject_cast<QLineEdit*>(inputFields["author"]);
        article->setAuthor(authorEdit ? authorEdit->text() : "");

        auto categoryEdit = qobject_cast<QLineEdit*>(inputFields["category"]);
        article->setCategory(categoryEdit ? categoryEdit->text() : "");

        auto urlEdit = qobject_cast<QLineEdit*>(inputFields["url"]);
        article->setUrl(urlEdit ? urlEdit->text() : "");

        auto dateEdit = qobject_cast<QDateEdit*>(inputFields["date"]);
        article->setDate(dateEdit ? dateEdit->date() : QDate());

        auto wordCountSpin = qobject_cast<QSpinBox*>(inputFields["wordCount"]);
        article->setWordCount(wordCountSpin ? wordCountSpin->value() : 0);

        if (auto academicArticle = dynamic_cast<AcademicArticle*>(media)) {
            auto universityNameEdit = qobject_cast<QLineEdit*>(inputFields["universityName"]);
            academicArticle->setUniversityName(universityNameEdit ? universityNameEdit->text() : "");

            auto citationCountSpin = qobject_cast<QSpinBox*>(inputFields["citationCount"]);
            academicArticle->setCitationCount(citationCountSpin ? citationCountSpin->value() : 0);

            auto peerReviewedCombo = qobject_cast<QComboBox*>(inputFields["peerReviewed"]);
            academicArticle->setPeerReviewed(peerReviewedCombo ? (peerReviewedCombo->currentIndex() == 1) : false);
        } else {
            auto newspaperArticle = static_cast<NewspaperArticle*>(media);
            auto headlineEdit = qobject_cast<QLineEdit*>(inputFields["headline"]);
            newspaperArticle->setHeadline(headlineEdit ? headlineEdit->text() : "");

            auto politicsCombo = qobject_cast<QComboBox*>(inputFields["politics"]);
            newspaperArticle->setPolitics(politicsCombo ? (politicsCombo->currentIndex() == 1) : false);
        }
    }
    return true;
}

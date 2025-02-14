#include "JSONVisitor.h"
#include "Literature.h"
#include "LiteratureClasses/Book.h"
#include "LiteratureClasses/Poem.h"
#include "Articles.h"
#include "ArticlesClasses/AcademicArticle.h"
#include "ArticlesClasses/NewspaperArticle.h"
#include "Media.h"
// TBD: explain in the doc how polymorhism is used here: it calls at runtime the right method

void JSONVisitor::visit(Media *media) {
    // QJsonObject expects an int, not an unsigned int
    json["id"] = static_cast<int>(media->getId());
    json["title"] = media->getTitle();
    json["short_description"] = media->getShortDescription();
    json["image_path"] = media->getImagePath();
}

void JSONVisitor::visit(Literature* literature) {
    visit(static_cast<Media*>(literature));
    json["author"] = literature->getAuthor();
    json["long_description"] = literature->getLongDescription();
    json["publication_year"] = literature->getPublicationYear();
    json["rating"] = literature->getRating();
}

void JSONVisitor::visit(Book* book) {
    visit(static_cast<Literature*>(book));
    json["publishing"] = book->getPublishing();
    // QJsonObject expects an int, not an unsigned int
    json["pages"] = static_cast<int>(book->getPages());
}

void JSONVisitor::visit(Poem* poem) {
    visit(static_cast<Literature*>(poem));
    json["city"] = poem->getCity();
}

void JSONVisitor::visit(Articles* article) {
    visit(static_cast<Media*>(article));
    json["author"] = article->getAuthor();
    json["category"] = article->getCategory();
    json["url"] = article->getUrl();
    json["date"] = article->getDate().toString(Qt::ISODate);
    json["word_count"] = static_cast<int>(article->getWordCount());
}

void JSONVisitor::visit(AcademicArticle* academicArticle) {
    visit(static_cast<Articles*>(academicArticle));
    json["university_name"] = academicArticle->getUniversityName();
    json["citation_count"] = static_cast<int>(academicArticle->getCitationCount());
    json["peer_reviewed"] = academicArticle->isPeerReviewed();
}

void JSONVisitor::visit(NewspaperArticle* newspaperArticle) {
    visit(static_cast<Articles*>(newspaperArticle)); // Call the Articles visit method
    json["headline"] = newspaperArticle->getHeadline();
    json["politics"] = newspaperArticle->isPolitics();
}

QJsonObject JSONVisitor::getJson() const {
    return json;
}

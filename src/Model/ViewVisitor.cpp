#include "ViewVisitor.h"

ViewVisitor::ViewVisitor(QLayout* layout) : layout(layout) {}

void ViewVisitor::addLabel(const QString& text, const QString& style) const {
    auto* label = new QLabel(text);
    label->setStyleSheet(style);
    label->setWordWrap(true);
    layout->addWidget(label);
}

void ViewVisitor::addTextBlock(const QString& text) const {
    auto* textEdit = new QTextEdit(text);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("border-radius: 10px; padding: 20px;");
    layout->addWidget(textEdit);
}

void ViewVisitor::visit(Media* media) {
    addLabel(media->getTitle(), "font-size: 30px; font-weight: bold;");
    addLabel(media->getShortDescription());
}

void ViewVisitor::visit(Literature* literature) {
    visit(static_cast<Media*>(literature));
    addLabel("Author: " + literature->getAuthor());
    addTextBlock("\n" + literature->getLongDescription());
    addLabel("Year: " + QString::number(literature->getPublicationYear()));
    addLabel("Rating: " + QString::number(literature->getRating()) + "/5");
}

void ViewVisitor::visit(Articles* article) {
    visit(static_cast<Media*>(article));
    addLabel("Author: " + article->getAuthor());
    addLabel("Category: " + article->getCategory());
    addLabel("URL: " + article->getUrl());
    addLabel("Date: " + article->getDate().toString());
    addLabel("Word Count: " + QString::number(article->getWordCount()));
}

void ViewVisitor::visit(Book* book) {
    visit(static_cast<Literature*>(book));
    addLabel("Publisher: " + book->getPublishing());
    addLabel("Pages: " + QString::number(book->getPages()));
}

void ViewVisitor::visit(Poem* poem) {
    visit(static_cast<Literature*>(poem));
    addLabel("Written in: " + poem->getCity());
    //addTextBlock(poem->getLongDescription());
}

void ViewVisitor::visit(AcademicArticle* academicArticle) {
    visit(static_cast<Articles*>(academicArticle));
    addLabel("University: " + academicArticle->getUniversityName());
    addLabel("Citations: " + QString::number(academicArticle->getCitationCount()));
    addLabel("is Peer Reviewed? " + QString(academicArticle->isPeerReviewed() ? "Yes" : "No"));
}

void ViewVisitor::visit(NewspaperArticle* newspaperArticle) {
    visit(static_cast<Articles*>(newspaperArticle));
    addLabel("Headline: " + newspaperArticle->getHeadline(), "font-size: 30px; font-weight: bold;");
    addLabel("is About Politics?: " + QString(newspaperArticle->isPolitics() ? "Yes" : "No! :)"));
}

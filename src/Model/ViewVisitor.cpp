#include "ViewVisitor.h"

ViewVisitor::ViewVisitor(QLayout* layout) : layout(layout) {}

void ViewVisitor::addLabel(const QString& text, const QString& style) const {
    QLabel* label = new QLabel(text);
    label->setStyleSheet(style);
    label->setWordWrap(true);
    layout->addWidget(label);
}

void ViewVisitor::addTextBlock(const QString& text) const {
    QTextEdit* textEdit = new QTextEdit(text);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("background-color: #f5f5f5; border-radius: 8px; padding: 10px;");
    layout->addWidget(textEdit);
}

void ViewVisitor::visit(Media* media) {
    addLabel("Title: " + media->getTitle(), "font-size: 24px; font-weight: bold;");
    addLabel("Description: " + media->getShortDescription());
}

void ViewVisitor::visit(Literature* literature) {
    visit(static_cast<Media*>(literature));
    addLabel("Author: " + literature->getAuthor());
    addTextBlock("Long Description:\n" + literature->getLongDescription());
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
    addLabel("City of Origin: " + poem->getCity());
    addTextBlock(poem->getLongDescription());
}

void ViewVisitor::visit(AcademicArticle* academicArticle) {
    visit(static_cast<Articles*>(academicArticle));
    addLabel("University: " + academicArticle->getUniversityName());
    addLabel("Citations: " + QString::number(academicArticle->getCitationCount()));
    addLabel("Peer Reviewed: " + QString(academicArticle->isPeerReviewed() ? "Yes" : "No"));
}

void ViewVisitor::visit(NewspaperArticle* newspaperArticle) {
    visit(static_cast<Articles*>(newspaperArticle));
    addLabel("Headline: " + newspaperArticle->getHeadline(), "font-size: 20px; font-weight: bold;");
    addLabel("Politics Section: " + QString(newspaperArticle->isPolitics() ? "Yes" : "No"));
}

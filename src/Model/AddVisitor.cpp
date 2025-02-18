#include <QLabel>
#include <QLineEdit>
#include "AddVisitor.h"

AddVisitor::AddVisitor(QWidget* widget) : widget(widget) {
    if (!widget->layout()) {
        widget->setLayout(new QVBoxLayout(widget));
    } else layout = widget->layout();
}

void AddVisitor::visit(Media* media) {
    layout->addWidget(new QLabel("Title:"));
    layout->addWidget(new QLineEdit(media->getTitle()));
    layout->addWidget(new QLabel("Short Description:"));
    layout->addWidget(new QLineEdit(media->getShortDescription()));
    layout->addWidget(new QLabel("Image Path:"));
    layout->addWidget(new QLineEdit(media->getImagePath()));
}

void AddVisitor::visit(Literature* literature) {
    visit(static_cast<Media*>(literature));
    layout->addWidget(new QLabel("Author:"));
    layout->addWidget(new QLineEdit(literature->getAuthor()));
    layout->addWidget(new QLabel("Long Description:"));
    layout->addWidget(new QLineEdit(literature->getLongDescription()));
    layout->addWidget(new QLabel("Publication Year:"));
    layout->addWidget(new QLineEdit(QString::number(literature->getPublicationYear())));
    layout->addWidget(new QLabel("Rating:"));
    layout->addWidget(new QLineEdit(QString::number(literature->getRating())));
}

void AddVisitor::visit(Articles* article) {
    visit(static_cast<Media*>(article));
    layout->addWidget(new QLabel("Author:"));
    layout->addWidget(new QLineEdit(article->getAuthor()));
    layout->addWidget(new QLabel("Category:"));
    layout->addWidget(new QLineEdit(article->getCategory()));
    layout->addWidget(new QLabel("URL:"));
    layout->addWidget(new QLineEdit(article->getUrl()));
    layout->addWidget(new QLabel("Date:"));
    layout->addWidget(new QLineEdit(article->getDate().toString(Qt::ISODate)));
    layout->addWidget(new QLabel("Word Count:"));
    layout->addWidget(new QLineEdit(QString::number(article->getWordCount())));
}

void AddVisitor::visit(Book* book) {
    visit(static_cast<Literature*>(book));
    layout->addWidget(new QLabel("Publishing:"));
    layout->addWidget(new QLineEdit(book->getPublishing()));
    layout->addWidget(new QLabel("Pages:"));
    layout->addWidget(new QLineEdit(QString::number(book->getPages())));
}

void AddVisitor::visit(Poem* poem) {
    visit(static_cast<Literature*>(poem));
    layout->addWidget(new QLabel("City:"));
    layout->addWidget(new QLineEdit(poem->getCity()));
}

void AddVisitor::visit(AcademicArticle* academicArticle) {
    visit(static_cast<Articles*>(academicArticle));
    layout->addWidget(new QLabel("University Name:"));
    layout->addWidget(new QLineEdit(academicArticle->getUniversityName()));
    layout->addWidget(new QLabel("Citation Count:"));
    layout->addWidget(new QLineEdit(QString::number(academicArticle->getCitationCount())));
    layout->addWidget(new QLabel("Peer Reviewed:"));
    layout->addWidget(new QLineEdit(academicArticle->isPeerReviewed() ? "Yes" : "No"));
}

void AddVisitor::visit(NewspaperArticle* newspaperArticle) {
    visit(static_cast<Articles*>(newspaperArticle));
    layout->addWidget(new QLabel("Headline:"));
    layout->addWidget(new QLineEdit(newspaperArticle->getHeadline()));
    layout->addWidget(new QLabel("Politics:"));
    layout->addWidget(new QLineEdit(newspaperArticle->isPolitics() ? "Yes" : "No"));
}

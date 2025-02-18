#ifndef ADDVISITOR_H
#define ADDVISITOR_H
#include <QWidget>
#include <QLayout>
#include "Visitor.h"
#include "Literature.h"
#include "Articles.h"
#include "LiteratureClasses/Book.h"
#include "LiteratureClasses/Poem.h"
#include "ArticlesClasses/AcademicArticle.h"
#include "ArticlesClasses/NewspaperArticle.h"

class AddVisitor : public Visitor {
private:
    QWidget* widget;
    QLayout* layout;
    // the layout will be taken from the widget, specifically
    // from CreateMediaWidget, and it will be a QVBoxLayout
public:
    explicit AddVisitor(QWidget* widget);
    void visit(Media* media) override;
    virtual void visit(Literature* literature);
    virtual void visit(Articles* article);
    virtual void visit(Book* book);
    virtual void visit(Poem* poem);
    virtual void visit(AcademicArticle* academicArticle);
    virtual void visit(NewspaperArticle* newspaperArticle);
};

#endif //ADDVISITOR_H

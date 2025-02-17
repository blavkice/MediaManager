#ifndef ADDVISITOR_H
#define ADDVISITOR_H
#include <QWidget>
#include <QVBoxLayout>
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
    QVBoxLayout* layout;
public:
    explicit AddVisitor(QWidget* widget);
    virtual void visit(Media* media) override;
    virtual void visit(Literature* literature);
    virtual void visit(Articles* article);
    virtual void visit(Book* book);
    virtual void visit(Poem* poem);
    virtual void visit(AcademicArticle* academicArticle);
    virtual void visit(NewspaperArticle* newspaperArticle);
};

#endif //ADDVISITOR_H

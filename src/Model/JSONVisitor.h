#ifndef JSONVISITOR_H
#define JSONVISITOR_H
#include <QJSonObject>
#include "Visitor.h"
#include "LiteratureClasses/Book.h"
#include "LiteratureClasses/Poem.h"
#include "ArticlesClasses/AcademicArticle.h"
#include "ArticlesClasses/NewspaperArticle.h"

class JSONVisitor : public Visitor {
private:
    QJsonObject json;
public:
    void visit(Media* media) override;
    void visit(Literature* literature) override;
    void visit(Articles* article) override;
    void visit(Book* book) override;
    void visit(Poem* poem) override;
    void visit(AcademicArticle* academicArticle) override;
    void visit(NewspaperArticle* newspaperArticle) override;
    QJsonObject getJson() const;
};

#endif //JSONVISITOR_H

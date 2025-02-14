#ifndef JSONVISITOR_H
#define JSONVISITOR_H
#include <QJSonObject>
#include "Visitor.h"

class JSONVisitor : public Visitor {
private:
    QJsonObject json;
public:
    void visit(Media* media) override;
    void visit(Literature* literature);
    void visit(Book* book);
    void visit(Poem* poem);
    void visit(Articles* article);
    void visit(AcademicArticle* academicArticle);
    void visit(NewspaperArticle* newspaperArticle);
    QJsonObject getJson() const;
};

#endif //JSONVISITOR_H

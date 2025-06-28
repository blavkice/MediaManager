#ifndef VISITOR_H
#define VISITOR_H
#include <QObject>

class Media;
class Literature;
class Articles;
class Book;
class Poem;
class AcademicArticle;
class NewspaperArticle;

class Visitor : public QObject {
    Q_OBJECT
   public:
    virtual ~Visitor() = default;
    virtual void visit(Media* media) = 0;
    virtual void visit(Literature* literature) = 0;
    virtual void visit(Articles* article) = 0;
    virtual void visit(Book* book) = 0;
    virtual void visit(Poem* poem) = 0;
    virtual void visit(AcademicArticle* academicArticle) = 0;
    virtual void visit(NewspaperArticle* newspaperArticle) = 0;
};

#endif  // VISITOR_H

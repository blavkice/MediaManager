#ifndef VISITOR_H
#define VISITOR_H

class Media;
class Literature;
class Articles;
class Book;
class Poem;
class AcademicArticle;
class NewspaperArticle;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(Media *media) = 0;
};

#endif //VISITOR_H

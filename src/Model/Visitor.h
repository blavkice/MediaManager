#ifndef VISITOR_H
#define VISITOR_H

class Media;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(Media *media) = 0;
};

#endif //VISITOR_H

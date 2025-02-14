#ifndef VISITABLE_H
#define VISITABLE_H

class Visitor;

class Visitable {
public:
    virtual ~Visitable() = default;
    virtual void accept(Visitor *visitor) = 0;
};

#endif //VISITABLE_H

#ifndef ADDVISITOR_H
#define ADDVISITOR_H
#include <QLayout>
#include <QLineEdit>
#include <QMap>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "Articles.h"
#include "ArticlesClasses/AcademicArticle.h"
#include "ArticlesClasses/NewspaperArticle.h"
#include "Literature.h"
#include "LiteratureClasses/Book.h"
#include "LiteratureClasses/Poem.h"
#include "Visitor.h"

class AddVisitor : public Visitor {
    Q_OBJECT
   private:
    QLayout* layout;
    QMap<QString, QWidget*> inputFields;
    QPushButton* chooseButton;

   public:
    explicit AddVisitor(QLayout* layout);
    void chooseImage();
    void visit(Media* media) override;
    void visit(Literature* literature) override;
    void visit(Articles* article) override;
    void visit(Book* book) override;
    void visit(Poem* poem) override;
    void visit(AcademicArticle* academicArticle) override;
    void visit(NewspaperArticle* newspaperArticle) override;
    bool saveInput(Media* media);
};

#endif  // ADDVISITOR_H

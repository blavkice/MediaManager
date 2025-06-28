#ifndef VIEWVISITOR_H
#define VIEWVISITOR_H
#include <QCoreApplication>
#include <QDir>
#include <QGroupBox>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QTextEdit>

#include "Articles.h"
#include "ArticlesClasses/AcademicArticle.h"
#include "ArticlesClasses/NewspaperArticle.h"
#include "Literature.h"
#include "LiteratureClasses/Book.h"
#include "LiteratureClasses/Poem.h"
#include "Visitor.h"

class ViewVisitor : public Visitor {
   private:
    QLayout* layout;
    void addLabel(const QString& text, const QString& style = "font-size:16px;") const;
    void addTextBlock(const QString& text) const;

   public:
    explicit ViewVisitor(QLayout* layout);
    void visit(Media* media) override;
    void visit(Literature* literature) override;
    void visit(Articles* article) override;
    void visit(Book* book) override;
    void visit(Poem* poem) override;
    void visit(AcademicArticle* academicArticle) override;
    void visit(NewspaperArticle* newspaperArticle) override;
};
#endif  // VIEWVISITOR_H

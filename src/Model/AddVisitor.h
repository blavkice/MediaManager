#ifndef ADDVISITOR_H
#define ADDVISITOR_H
#include <QWidget>
#include <QLayout>
#include <QMap>
#include <QLineEdit>
#include <QObject>
#include "Visitor.h"
#include "Literature.h"
#include "Articles.h"
#include "LiteratureClasses/Book.h"
#include "LiteratureClasses/Poem.h"
#include "ArticlesClasses/AcademicArticle.h"
#include "ArticlesClasses/NewspaperArticle.h"

class AddVisitor : public Visitor {
    Q_OBJECT
private:
    QLayout* layout;
    QMap<QString, QLineEdit*> inputFields;
    QPushButton* chooseButton;
public:
    explicit AddVisitor(QLayout* layout, QWidget* parent = nullptr, QObject* obj_parent = nullptr);
    void chooseImage();
    void visit(Media* media) override;
    void visit(Literature* literature) override;
    void visit(Articles* article) override;
    void visit(Book* book) override;
    void visit(Poem* poem) override;
    void visit(AcademicArticle* academicArticle) override;
    void visit(NewspaperArticle* newspaperArticle) override;
    bool saveInput(Media* media);
    static void saveImage(Media* media, const QString& imagePath);
};

#endif // ADDVISITOR_H

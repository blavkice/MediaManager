#ifndef JSONVISITOR_H
#define JSONVISITOR_H
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

#include "../View/MediaListController.h"
#include "Visitor.h"

class JSONVisitor : public Visitor {
   public:
    explicit JSONVisitor(MediaListController* mediaListController);
    bool importFromFile(const QString& filePath) const;
    bool exportToFile(const QString& filePath);
    void visit(Media* media) override;
    void visit(Literature* literature) override;
    void visit(Articles* article) override;
    void visit(Book* book) override;
    void visit(Poem* poem) override;
    void visit(AcademicArticle* academicArticle) override;
    void visit(NewspaperArticle* newspaperArticle) override;
    static Media* deserialize(const QJsonObject& json);

   private:
    MediaListController* mediaListController;
    mutable QJsonArray jsonArray;
};

#endif  // JSONVISITOR_H

#ifndef MENUBAR_H
#define MENUBAR_H
#include <QAction>
#include <QMenuBar>

#include "../Model/JSONVisitor.h"

class MenuBar : public QMenuBar {
    Q_OBJECT
   private:
    QMenu* actionsMenu;
    QAction* importAction;
    QAction* exportAction;
    QAction* saveAction;
    JSONVisitor* jsonVisitor;
    static void replaceLastDefaultMedia(const QString& filePath);

   public:
    explicit MenuBar(QWidget* parent);
    QAction* getImportAction() const;
    QAction* getExportAction() const;
    void setJSONVisitor(JSONVisitor* jsonVisitor);
    void importLastDefaultMedia();
   signals:
    void mediaImported();
   public slots:
    void onImportActionTriggered();
    void onExportActionTriggered();
    void onSaveActionTriggered();
};

#endif  // MENUBAR_H

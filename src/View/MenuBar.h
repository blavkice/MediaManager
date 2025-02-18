#ifndef MENUBAR_H
#define MENUBAR_H
#include <QMenuBar>
#include <QAction>
#include "../Model/JSONVisitor.h"

class MenuBar : public QMenuBar {
    Q_OBJECT
private:
    QMenu* actionsMenu;
    QAction* exit;
    QAction* importAction;
    QAction* exportAction;
    JSONVisitor* jsonVisitor;
public:
    explicit MenuBar(QWidget* parent);
    QAction* getExit() const;
    QAction* getImportAction() const;
    QAction* getExportAction() const;
    void setJSONVisitor(JSONVisitor* jsonVisitor);
signals:
    void mediaImported();
public slots:
    void onImportActionTriggered();
    void onExportActionTriggered();
};

#endif //MENUBAR_H

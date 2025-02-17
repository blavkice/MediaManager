#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMenu>
// ReSharper disable once CppUnusedIncludeDirective
#include <QMenuBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include "Controller/MediaListController.h"

// ReSharper disable once CppClassCanBeFinal
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    QWidget* centralWidget;
    QWidget* rightInfoWidget;
    QMenuBar* menuBar;
    QMenu* actionsMenu;
    QAction* exit;
    QAction* save; // TBD real persistence
    void initMenuBar();
    void initLayouts();
    QHBoxLayout* hMainViewLayout;
    QVBoxLayout* vLeftLayout;
    QLineEdit* searchBox;
    QListView* listView;
    QGridLayout* gridLayout;
    // to populate the list view
    MediaListController* mediaListController;

public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif //MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "View/MediaListController.h"

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
    // vertical left layout elements
    QVBoxLayout* vLeftLayout;
    QLineEdit* searchBox;
    QPushButton* addButton;
    QPushButton* removeButton;

    QListView* listView;
    QGridLayout* gridLayout;
    // to populate the list view
    MediaListController* mediaListController;

public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif //MAINWINDOW_H

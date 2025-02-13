#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

// ReSharper disable once CppClassCanBeFinal
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    QWidget* centralWidget;
    QMenuBar* menuBar;
    QMenu* actionsMenu;
    QAction* exit;
    QAction* save; // TBD real persistence
    void initMenuBar();
public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif //MAINWINDOW_H

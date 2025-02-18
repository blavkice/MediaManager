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
#include <QComboBox>
#include "View/MediaListController.h"
#include "View/RightDynamicWidget.h"

// ReSharper disable once CppClassCanBeFinal
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    QWidget* centralWidget;
    RightDynamicWidget* rightInfoWidget;
    QMenuBar* menuBar;
    QMenu* actionsMenu;
    QAction* exit;
    QAction* save; // TBD real persistence
    void initMenuBar();
    void initLayouts();
    void initAddComboBox();
    QHBoxLayout* hMainViewLayout;
    // vertical left layout elements
    QVBoxLayout* vLeftLayout;
    QLineEdit* searchBox;
    QPushButton* addButton;
    QPushButton* removeButton;
    QComboBox* addComboBox;
    QListView* listView;
    // to populate the list view
    MediaListController* mediaListController;
private slots:
    void onAddButtonClicked() const;
    void onComboBoxActivated(int index) const;
    void onMediaSelected(int index) const;
    void onMediaCreated(Media* media) const;
public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif //MAINWINDOW_H

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
#include "View/MenuBar.h"
#include "View/MediaListController.h"
#include "View/RightDynamicWidget.h"

// ReSharper disable once CppClassCanBeFinal
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    QWidget* centralWidget;
    // the two personalized elements
    RightDynamicWidget* rightInfoWidget;
    MenuBar* menuBar;
    QHBoxLayout* hMainViewLayout;
    // vertical left layout elements
    QVBoxLayout* vLeftLayout;
    QLineEdit* searchBox;
    QPushButton* addButton;
    QPushButton* removeButton;
    QComboBox* addComboBox;
    QListView* listView;
    // for the list of currently active elements
    MediaListController* mediaListController;
    void initLayouts();
    void initAddComboBox();
private slots:
    void onAddButtonClicked() const;
    void onComboBoxActivated(int index) const;
    void onMediaSelected(int index) const;
    void onMediaCreated(Media* media) const;
public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif //MAINWINDOW_H

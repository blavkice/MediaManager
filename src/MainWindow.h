#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>

#include "View/MediaListController.h"
#include "View/MenuBar.h"
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
    QComboBox* typeFilterBox;
    QPushButton* addButton;
    QPushButton* removeButton;
    QComboBox* addComboBox;
    QListView* listView;
    // for the list of currently active elements
    MediaListController* mediaListController;
    void initLayouts();
    void initAddComboBox();
    void onRemoveButtonClicked();
   private slots:
    void onAddButtonClicked() const;
    void onComboBoxActivated(int index) const;
    void onMediaSelected(int index) const;
    void onMediaCreated(Media* media) const;
    void updateSelectionState(bool selected) const;
    void onMediaEdited(Media* media) const;

   public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif  // MAINWINDOW_H

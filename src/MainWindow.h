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
#include <QStackedWidget>

#include "View/MediaListController.h"
#include "View/MenuBar.h"
#include "View/RightDynamicWidget.h"
#include "View/GridView.h"

// ReSharper disable once CppClassCanBeFinal
class MainWindow : public QMainWindow {
    Q_OBJECT
   private:
    QWidget* toolsWidget;
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
    // for the grid view
    QStackedWidget* centralStack;
    QWidget* splitViewWidget;
    GridView* gridView;
    QPushButton* gridViewButton;
    QPushButton* splitViewButton;
    enum ViewMode { Split, FullscreenGrid, FullscreenDetail };
    ViewMode currentViewMode = Split;
    QWidget* detailWidget = nullptr; // for the detail view in fullscreen mode
    // for the list of currently active elements
    MediaListController* mediaListController;
    void initLayouts();
    void initAddComboBox();
    void onRemoveButtonClicked();
    // to switch between the two views
    void switchToGridView();
    void switchToSplitView();
    void showFullscreenDetail(const QModelIndex& index);
   private slots:
    void onAddButtonClicked() const;
    void onComboBoxActivated(int index) const;
    void onMediaSelected(int index) const;
    void onMediaCreated(Media* media) const;
    void updateSelectionState(bool selected) const;
    void onMediaEdited(Media* media);
   public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif  // MAINWINDOW_H

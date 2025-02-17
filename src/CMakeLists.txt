cmake_minimum_required(VERSION 3.30)
project(MediaManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
  Core
  Gui
  Widgets
  REQUIRED)

add_executable(MediaManager main.cpp
        Model/Media.cpp
        Model/Media.h
        Model/Literature.cpp
        Model/Literature.h
        Model/LiteratureClasses/Book.cpp
        Model/LiteratureClasses/Book.h
        Model/LiteratureClasses/Poem.cpp
        Model/LiteratureClasses/Poem.h
        Model/Articles.cpp
        Model/Articles.h
        Model/ArticlesClasses/AcademicArticle.cpp
        Model/ArticlesClasses/AcademicArticle.h
        Model/ArticlesClasses/NewspaperArticle.cpp
        Model/ArticlesClasses/NewspaperArticle.h
        MainWindow.cpp
        MainWindow.h
        Model/Visitor.h
        Model/Visitable.h
        Model/JSONVisitor.cpp
        Model/JSONVisitor.h
        View/MediaListController.cpp
        View/MediaListController.h
        View/InfoListPainter.cpp
        View/InfoListPainter.h
        resources.qrc
        View/StyleManager.cpp
        View/StyleManager.h
        Model/AddVisitor.cpp
        Model/AddVisitor.h
        View/CreateMediaWidget.cpp
        View/CreateMediaWidget.h)
target_link_libraries(MediaManager
  Qt::Core
  Qt::Gui
  Qt::Widgets
)


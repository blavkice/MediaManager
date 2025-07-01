QT       += core gui widgets

TARGET = MediaManager
TEMPLATE = app

# versione minima qt, per sicurezza...
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

# file di progetto
SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Model/AddVisitor.cpp \
    Model/Articles.cpp \
    Model/ArticlesClasses/AcademicArticle.cpp \
    Model/ArticlesClasses/NewspaperArticle.cpp \
    Model/EditVisitor.cpp \
    Model/JSONVisitor.cpp \
    Model/Literature.cpp \
    Model/LiteratureClasses/Book.cpp \
    Model/LiteratureClasses/Poem.cpp \
    Model/Media.cpp \
    Model/ViewVisitor.cpp \
    View/CreateMediaWidget.cpp \
    View/GridView.cpp \
    View/InfoListPainter.cpp \
    View/MediaFilterController.cpp \
    View/MediaListController.cpp \
    View/MenuBar.cpp \
    View/RightDynamicWidget.cpp \
    View/StyleManager.cpp \
    View/ViewMediaWidget.cpp

HEADERS += \
    MainWindow.h \
    Model/AddVisitor.h \
    Model/Articles.h \
    Model/ArticlesClasses/AcademicArticle.h \
    Model/ArticlesClasses/NewspaperArticle.h \
    Model/EditVisitor.h \
    Model/JSONVisitor.h \
    Model/Literature.h \
    Model/LiteratureClasses/Book.h \
    Model/LiteratureClasses/Poem.h \
    Model/Media.h \
    Model/ViewVisitor.h \
    Model/Visitable.h \
    Model/Visitor.h \
    View/CreateMediaWidget.h \
    View/GridView.h \
    View/InfoListPainter.h \
    View/MediaFilterController.h \
    View/MediaListController.h \
    View/MenuBar.h \
    View/RightDynamicWidget.h \
    View/StyleManager.h \
    View/ViewMediaWidget.h

RESOURCES += resources.qrc

# meglio specificare dove sono le risorse
DISTFILES += \
    default.jpg \
    Fonts/AceSansExtrabold.ttf \
    Fonts/cmu.concrete-roman.ttf \
    Fonts/cmu.serif-bold.ttf \
    Fonts/Quicksand-Regular-400.ttf

# per sicurezza, anche se è tutto sotto Model e View
INCLUDEPATH += $$PWD/Model $$PWD/Model/ArticlesClasses $$PWD/Model/LiteratureClasses $$PWD/View

# rimuove warning non intelligenti su macOS
macx: QMAKE_LFLAGS += -framework ApplicationServices

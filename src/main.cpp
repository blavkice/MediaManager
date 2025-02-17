#include <QApplication>
#include "MainWindow.h"
#include "View/StyleManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    StyleManager::applyMaterialStyle(app);
    StyleManager::setFont(app);

    MainWindow w;
    w.show();
    return QApplication::exec();
}

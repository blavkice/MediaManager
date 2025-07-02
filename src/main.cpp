#include <QApplication>
#include <QDir>

#include "MainWindow.h"
#include "View/StyleManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    StyleManager::applyMaterialStyle(app);
    StyleManager::setFont(app);

    // to allow the use of "std::shared_ptr<Media>" in qt signals and slots
    // specifically, in order to use it in the search filter inside MediaListController
    qRegisterMetaType<std::shared_ptr<Media>>("std::shared_ptr<Media>");

    const QString basePath = QCoreApplication::applicationDirPath();

    const QDir dir(basePath);
    if (!dir.exists("media")) dir.mkdir("media");
    if (!dir.exists("saves")) dir.mkdir("saves");

    MainWindow w;
    w.show();
    return QApplication::exec();
}

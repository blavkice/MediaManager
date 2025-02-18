#ifndef JSONEDITOR_H
#define JSONEDITOR_H
#include <QString>
#include <QJsonObject>
#include "../View/MediaListController.h"

class JSONEditor {
public:
    explicit JSONEditor(MediaListController* mediaListController);
    bool importFromFile(const QString& filePath);
    bool exportToFile(const QString& filePath);
private:
    MediaListController* mediaListController;
    static void mediaToJson(const Media* media, QJsonObject& json);
    static Media* jsonToMedia(const QJsonObject& json);
};

#endif // JSONEDITOR_H

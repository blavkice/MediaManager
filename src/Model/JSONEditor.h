#ifndef JSONVISITOR_H
#define JSONVISITOR_H
#include <QList>
#include <QString>
#include <QJsonObject>
#include "Media.h"

class JSONEditor {
public:
    bool importFromFile(const QString& filePath);
    bool exportToFile(const QString& filePath);
    QList<Media*> getMediaList() const;
private:
    QList<Media*> mediaList;
    static void mediaToJson(const Media* media, QJsonObject& json);
    static Media* jsonToMedia(const QJsonObject& json);
};

#endif // JSONVISITOR_H

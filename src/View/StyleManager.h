#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H
#include <QApplication>
#include <QFontDatabase>
#include <QPalette>
#include <QStyleFactory>

class StyleManager {
   public:
    static void applyMaterialStyle(QApplication& app);
    static void setFont(QApplication& app);
};

#endif  // STYLEMANAGER_H

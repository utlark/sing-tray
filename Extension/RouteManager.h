#ifndef SING_TRAY_ROUTEMANAGER_H
#define SING_TRAY_ROUTEMANAGER_H

#pragma once

#include <QCryptographicHash>
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QDir>

#include "Cryptographic.h"

class RouteManager : public QObject {
Q_OBJECT

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"

signals:

    void routeChanged();

#pragma clang diagnostic pop

public:
    explicit RouteManager(QObject *parent, QMenu *menu, const QString &configFilePath);

    void LoadRoutes();

    void UpdateActiveRoute();

    QFile ConfigFile;
    QString RoutesDirPath = QCoreApplication::applicationDirPath() + "/Routes/";
private:
    QMenu *RoutesMenu;

    QStringList RoutesFilter = QStringList() << "*.json";
};

#endif //SING_TRAY_ROUTEMANAGER_H

#ifndef SING_TRAY_ROUTEMANAGER_H
#define SING_TRAY_ROUTEMANAGER_H

#pragma once

#include <QCryptographicHash>
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QDir>

#include "../Sys/SystemCtl.h"

class RouteManager {
public:
    explicit RouteManager(QMenu *menu);

    void LoadRoutes();

    void UpdateActiveRoute();

private:
    QMenu *routesMenu;
    QString destinationPath = "/etc/sing-box/config.json";
    QStringList routesFilter = QStringList() << "*.json";
};

#endif //SING_TRAY_ROUTEMANAGER_H

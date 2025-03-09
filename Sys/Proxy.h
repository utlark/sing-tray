#ifndef SING_TRAY_PROXY_H
#define SING_TRAY_PROXY_H

#pragma once

#include <QStandardPaths>
#include <QProcess>
#include <QString>
#include <QDebug>

class Proxy {
public:
    static void ClearSystemProxy();

    static void SetSystemProxy(const QString &address, int http_port, int socks_port);

private:
    static bool IsKde;
    static bool IsGnome;
    static QString ConfigPath;
};

#endif //SING_TRAY_PROXY_H

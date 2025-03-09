#ifndef SING_TRAY_CAP_H
#define SING_TRAY_CAP_H

#pragma once

#include <QStandardPaths>
#include <QProcess>
#include <QString>
#include <QDebug>

class Cap {
public:
    static QString GetCapString(const QString &path);

    static int SetCapString(const QString &path, const QString &cap);

    static bool CheckPkexecInstall();

    static QString FindProgExec(const QString &name);
};

#endif //SING_TRAY_CAP_H

#ifndef SING_TRAY_SYSTEMCTL_H
#define SING_TRAY_SYSTEMCTL_H

#pragma once

#include <QRegularExpression>
#include <QProcess>
#include <QString>

class SystemCtl {
public:
    static void Execute(const QString &serviceName, const QString &command);

    static bool CheckStatus(const QString &serviceName);
};

#endif //SING_TRAY_SYSTEMCTL_H

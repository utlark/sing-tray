#ifndef SING_TRAY_CORESERVICE_H
#define SING_TRAY_CORESERVICE_H

#pragma once

#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QDir>

#include "Proxy.h"
#include "Cap.h"

class CoreService : public QObject {
Q_OBJECT

public:
    explicit CoreService(QObject *parent);

    ~CoreService() override;

    void Start();

    void Stop();

    void Restart();

    bool IsRunning();

    QString WorkDir = QCoreApplication::applicationDirPath() + "/Core";
    QString BinPath = WorkDir + "/sing-box";

    QFile *ConfigFile = new QFile(WorkDir + "/config.json");
private:
    QProcess Process;
};

#endif //SING_TRAY_CORESERVICE_H

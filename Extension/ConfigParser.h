#ifndef SING_TRAY_CONFIGPARSER_H
#define SING_TRAY_CONFIGPARSER_H

#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>

class ConfigParser {
public:
    static QPair<QString, int> GetMixedInbound(const QString &configFilePath);
};


#endif //SING_TRAY_CONFIGPARSER_H

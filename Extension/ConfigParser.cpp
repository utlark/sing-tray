#include "ConfigParser.h"

QPair<QString, int> ConfigParser::GetMixedInbound(const QString &configFilePath) {
    QFile configFile(configFilePath);

    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Couldn't open the file: " << configFilePath;
        exit(1);
    }

    QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll());
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON: " << configFilePath;
        exit(1);
    }

    QJsonObject rootObj = doc.object();
    if (!rootObj.contains("inbounds") || !rootObj["inbounds"].isArray()) {
        qWarning() << "The 'inbounds' section was not found: " << configFilePath;
        exit(1);
    }

    QJsonArray inboundsArray = rootObj["inbounds"].toArray();
    for (const auto &value: inboundsArray) {
        if (!value.isObject())
            continue;

        QJsonObject obj = value.toObject();
        if (obj.value("type").toString() == "mixed") {
            QString listen = obj.value("listen").toString();
            int listenPort = obj.value("listen_port").toInt();

            qDebug() << "listen:" << listen;
            qDebug() << "listen_port:" << listenPort;
            return {listen, listenPort};
        }
    }

    return {};
}

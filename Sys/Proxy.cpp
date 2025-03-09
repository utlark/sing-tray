//
// The code of this class is taken from the project:
//      https://github.com/MatsuriDayo/nekoray/blob/main/3rdparty/qv2ray/v2/components/proxy/QvProxyConfigurator.cpp
//
//  Created by MatsuriDayo
//  Copyright © MatsuriDayo
//

#include "Proxy.h"

#define IntToQstring(num) QString::number(num)
using ProcessArgument = QPair<QString, QStringList>;

bool Proxy::IsKde = qEnvironmentVariable("XDG_SESSION_DESKTOP") == "KDE" || qEnvironmentVariable("XDG_SESSION_DESKTOP") == "plasma";
bool Proxy::IsGnome = qEnvironmentVariable("XDG_SESSION_DESKTOP") == "gnome";

QString Proxy::ConfigPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);

void Proxy::SetSystemProxy(const QString &address, int httpPort, int socksPort) {
    bool hasHTTP = (httpPort > 0 && httpPort < 65536);
    bool hasSOCKS = (socksPort > 0 && socksPort < 65536);

    if (!hasHTTP && !hasSOCKS)
        return;

    QList<ProcessArgument> actions;
    if (hasHTTP) {
        for (const auto &protocol: QStringList{"http", "https", "ftp"})
            if (IsKde) {
                actions << ProcessArgument{"kwriteconfig5", {"--file", ConfigPath + "/kioslaverc", "--group", "Proxy Settings", "--key", protocol + "Proxy",
                                                             "http://" + address + " " + IntToQstring(httpPort)}};
            } else if (IsGnome) {
                actions << ProcessArgument{"gsettings", {"set", "org.gnome.system.proxy." + protocol, "host", address}};
                actions << ProcessArgument{"gsettings", {"set", "org.gnome.system.proxy." + protocol, "port", IntToQstring(httpPort)}};
            }
    }

    if (hasSOCKS) {
        if (IsKde) {
            actions << ProcessArgument{"kwriteconfig5", {"--file", ConfigPath + "/kioslaverc", "--group", "Proxy Settings", "--key", "socksProxy",
                                                         "socks://" + address + " " + IntToQstring(socksPort)}};
        } else if (IsGnome) {
            actions << ProcessArgument{"gsettings", {"set", "org.gnome.system.proxy.socks", "host", address}};
            actions << ProcessArgument{"gsettings", {"set", "org.gnome.system.proxy.socks", "port", IntToQstring(socksPort)}};
        }
    }

    if (IsKde) {
        actions << ProcessArgument{"kwriteconfig5", {"--file", ConfigPath + "/kioslaverc", "--group", "Proxy Settings", "--key", "ProxyType", "1"}};
        actions << ProcessArgument{"dbus-send", {"--type=signal", "/KIO/Scheduler", "org.kde.KIO.Scheduler.reparseSlaveConfiguration", "string:''"}};
    } else if (IsGnome) {
        actions << ProcessArgument{"gsettings", {"set", "org.gnome.system.proxy", "mode", "manual"}};
    }

    QList<bool> results;
    for (const auto &action: actions) {
        int returnCode = QProcess::execute(action.first, action.second);
        qDebug() << QStringLiteral("[%1] Program: %2, Args: %3").arg(returnCode).arg(action.first, action.second.join(";"));
        results << (returnCode == QProcess::NormalExit);
    }

    if (results.count(true) != actions.size())
        qWarning() << "Something wrong when setting proxies.";
}

void Proxy::ClearSystemProxy() {
    QList<ProcessArgument> actions;

    if (IsKde) {
        actions << ProcessArgument{"kwriteconfig5", {"--file", ConfigPath + "/kioslaverc", "--group", "Proxy Settings", "--key", "ProxyType", "0"}};
        actions << ProcessArgument{"dbus-send", {"--type=signal", "/KIO/Scheduler", "org.kde.KIO.Scheduler.reparseSlaveConfiguration", "string:''"}};
    } else if (IsGnome) {
        actions << ProcessArgument{"gsettings", {"set", "org.gnome.system.proxy", "mode", "none"}};
    }

    for (const auto &action: actions) {
        int returnCode = QProcess::execute(action.first, action.second);
        qDebug() << QStringLiteral("[%1] Program: %2, Args: %3").arg(returnCode).arg(action.first, action.second.join(";"));
    }
}

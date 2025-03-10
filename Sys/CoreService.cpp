#include "CoreService.h"

CoreService::CoreService(QObject *parent) : QObject(parent) {
    if (!Cap::CheckPkexecInstall()) {
        qWarning() << "Please install \"pkexec\" first.";
        exit(1);
    }

    if (!Cap::GetCapString(BinPath).contains("cap_net_admin") && Cap::SetCapString(BinPath, "cap_net_admin=ep") != 0)
        exit(1);
}

CoreService::~CoreService() {
    Stop();
}

void CoreService::Start() {
    if (!CoreService::IsRunning()) {
        Process.start(BinPath, {"--directory", WorkDir, "--disable-color", "run"});

        auto proxySettings = ConfigParser::GetMixedInbound(ConfigFilePath);
        if (!proxySettings.first.isNull() && !proxySettings.first.isEmpty())
            Proxy::SetSystemProxy(proxySettings.first, proxySettings.second, proxySettings.second);

        if (Process.waitForStarted())
            qDebug() << "Sing-box started";
        else
            qWarning() << "Failed to start sing-box: " << Process.errorString();
    }
}

void CoreService::Stop() {
    if (CoreService::IsRunning()) {
        Process.kill();
        Proxy::ClearSystemProxy();

        if (Process.waitForFinished())
            qDebug() << "Sing-box stopped";
        else
            qWarning() << "Failed to stop sing-box: " << Process.errorString();
    }
}

void CoreService::Restart() {
    if (CoreService::IsRunning()) {
        Stop();
        Start();
    }
}

bool CoreService::IsRunning() {
    return Process.state() == QProcess::Running;
}

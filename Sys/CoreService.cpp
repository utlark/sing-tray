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
    delete ConfigFile;
}

void CoreService::Start() {
    if (!CoreService::IsRunning()) {
        process.start(BinPath, {"--directory", WorkDir, "--disable-color", "run"});

        if (process.waitForStarted())
            qDebug() << "Sing-box started";
        else
            qWarning() << "Failed to start sing-box: " << process.errorString();
    }
}

void CoreService::Stop() {
    if (CoreService::IsRunning()) {
        process.kill();

        if (process.waitForFinished())
            qDebug() << "Sing-box stopped";
        else
            qWarning() << "Failed to stop sing-box: " << process.errorString();
    }
}

void CoreService::Restart() {
    if (CoreService::IsRunning()) {
        Stop();
        Start();
    }
}

bool CoreService::IsRunning() {
    return process.state() == QProcess::Running;
}

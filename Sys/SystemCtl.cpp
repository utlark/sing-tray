#include "SystemCtl.h"

QRegularExpression regex(R"(Active:\s+active \(running\))");

void SystemCtl::Execute(const QString &serviceName, const QString &command) {
    QProcess process;

    process.start("pkexec", QStringList() << "systemctl" << command << serviceName);
    process.waitForFinished();

    if (process.exitCode() != 0)
        qWarning() << "Error while executing the command " << process.readAllStandardError();
}

bool SystemCtl::CheckStatus(const QString &serviceName) {
    QProcess process;

    process.start("systemctl", QStringList() << "status" << serviceName);
    process.waitForFinished();

    if (process.exitCode() == 0)
        return regex.match(process.readAllStandardOutput()).hasMatch();
    return false;
}

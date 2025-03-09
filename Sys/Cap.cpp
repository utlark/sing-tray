//
// The code of this class is taken from the project:
//      https://github.com/MatsuriDayo/nekoray/blob/main/sys/linux/LinuxCap.cpp
//
//  Created by MatsuriDayo
//  Copyright © MatsuriDayo
//

#include "Cap.h"

#define EXIT_CODE(p) (p.exitStatus() == QProcess::NormalExit ? p.exitCode() : -1)

QString Cap::GetCapString(const QString &path) {
    QProcess p;
    p.setProgram(Cap::FindProgExec("getcap"));
    p.setArguments({path});
    p.start();
    p.waitForFinished(500);
    return p.readAllStandardOutput();
}

int Cap::SetCapString(const QString &path, const QString &cap) {
    QProcess p;
    p.setProgram("pkexec");
    p.setArguments({Cap::FindProgExec("setcap"), cap, path});
    p.start();
    p.waitForFinished(-1);

    int exitCode = EXIT_CODE(p);
    if (exitCode != 0)
        qWarning() << "Setcap failed: " << p.readAllStandardError();
    return exitCode;
}

bool Cap::CheckPkexecInstall() {
    QProcess p;
    p.setProgram("pkexec");
    p.setArguments({"--help"});
    p.setProcessChannelMode(QProcess::SeparateChannels);
    p.start();
    p.waitForFinished(500);
    return EXIT_CODE(p) == 0;
}

QString Cap::FindProgExec(const QString &name) {
    QString exec = QStandardPaths::findExecutable(name);
    if (exec.isEmpty())
        exec = QStandardPaths::findExecutable(name, {"/usr/sbin", "/sbin"});

    if (exec.isEmpty())
        qWarning() << "Executable" << name << "could not be resolved";
    else
        qDebug() << "Found exec" << name << "at" << exec;

    QString result = exec.isEmpty() ? name : exec;
    return result;
}

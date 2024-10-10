#ifndef SING_TRAY_AUTORUN_H
#define SING_TRAY_AUTORUN_H

#pragma once

#include <QStandardPaths>
#include <QApplication>
#include <QTextStream>
#include <QDir>

class AutoRun {
public:
    static void SetEnabled(bool enable);

    static bool IsEnabled();

private:
    static QString GetUserAutostartDir();
};

#endif //SING_TRAY_AUTORUN_H

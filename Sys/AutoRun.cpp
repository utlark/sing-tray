#include "AutoRun.h"

#define NEWLINE "\n"

void AutoRun::SetEnabled(bool enable) {
    QString appName = QCoreApplication::applicationName();
    QString userAutoStartPath = GetUserAutostartDir();
    QString desktopFileLocation = userAutoStartPath + appName + QLatin1String(".desktop");

    if (enable) {
        if (!QDir().exists(userAutoStartPath) && !QDir().mkpath(userAutoStartPath)) {
            qWarning() << "Could not create autostart folder: " << userAutoStartPath;
            return;
        }

        QFile iniFile(desktopFileLocation);
        if (!iniFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Could not write auto start entry: " << desktopFileLocation;
            return;
        }

        QTextStream ts(&iniFile);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        ts.setCodec("UTF-8");
#endif
        ts << QLatin1String("[Desktop Entry]") << NEWLINE
           << QLatin1String("Name=") << appName << NEWLINE
           << QLatin1String("Exec=") << QApplication::applicationFilePath() << NEWLINE
           << QLatin1String("Terminal=") << "false" << NEWLINE
           << QLatin1String("Categories=") << "Network" << NEWLINE
           << QLatin1String("Type=") << "Application" << NEWLINE
           << QLatin1String("StartupNotify=") << "false" << NEWLINE
           << QLatin1String("X-GNOME-Autostart-enabled=") << "true" << NEWLINE;
        ts.flush();
        iniFile.close();
    } else {
        QFile::remove(desktopFileLocation);
    }
}

bool AutoRun::IsEnabled() {
    QString appName = QCoreApplication::applicationName();
    QString desktopFileLocation = GetUserAutostartDir() + appName + QLatin1String(".desktop");
    return QFile::exists(desktopFileLocation);
}

QString AutoRun::GetUserAutostartDir() {
    QString config = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    config += QLatin1String("/autostart/");
    return config;
}

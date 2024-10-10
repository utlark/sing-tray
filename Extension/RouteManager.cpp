#include "RouteManager.h"

RouteManager::RouteManager(QMenu *menu) : routesMenu(menu) {
    LoadRoutes();
}

QString CalculateCRC32(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file:" << filePath;
        return {};
    }
    return QString::fromLatin1(QCryptographicHash::hash(file.readAll(), QCryptographicHash::RealSha3_256).toHex());
}

void RouteManager::UpdateActiveRoute() {
    QString currentHash = CalculateCRC32(destinationPath);

    for (QAction *action: routesMenu->actions())
        if (CalculateCRC32(QCoreApplication::applicationDirPath() + "/Routes/" + action->text() + ".json") == currentHash)
            action->setChecked(true);
        else
            action->setChecked(false);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemoryLeak"

void RouteManager::LoadRoutes() {
    QDir appDir(QCoreApplication::applicationDirPath());
    QDir routesDir(appDir.filePath("Routes"));

    if (!routesDir.exists()) {
        qWarning() << "Directory 'Routes' does not exist.";
        return;
    }

    routesDir.setNameFilters(routesFilter);
    QFileInfoList fileList = routesDir.entryInfoList(QDir::Files, QDir::Name);

    if (fileList.isEmpty()) {
        qWarning() << "No .json files found in 'Routes' directory.";
        return;
    }

    routesMenu->clear();
    for (const QFileInfo &fileInfo: fileList) {
        auto *routeAction = new QAction(fileInfo.fileName().remove(".json"), routesMenu);
        routeAction->setCheckable(true);

        routesMenu->addAction(routeAction);

        QFileInfo localFileInfo = fileInfo;
        bool connectionSuccess = QObject::connect(routeAction, &QAction::triggered, [localFileInfo, this]() {
            QProcess process;

            process.start("pkexec", QStringList() << "cp" << localFileInfo.absoluteFilePath() << destinationPath);
            process.waitForFinished();

            if (process.exitCode() == 0) {
                SystemCtl::Execute("sing-box", "restart");
                UpdateActiveRoute();
            } else {
                qWarning() << "Failed to copy" << localFileInfo.fileName() << process.readAllStandardError();
            }
        });

        if (!connectionSuccess)
            qCritical() << "The signal could not be connected to the slot!";
    }

    UpdateActiveRoute();
}

#pragma clang diagnostic pop

#include "RouteManager.h"

RouteManager::RouteManager(QObject *parent, QMenu *menu, QFile *configFile) : QObject(parent), RoutesMenu(menu), ConfigFile(configFile) {
    LoadRoutes();

    if (!configFile->exists() && !RoutesMenu->actions().isEmpty())
        RoutesMenu->actions().first()->trigger();

    UpdateActiveRoute();
}

void RouteManager::UpdateActiveRoute() {
    QString currentHash;
    if (ConfigFile->exists())
        currentHash = Cryptographic::CalculateSha256(ConfigFile->fileName());

    for (QAction *action: RoutesMenu->actions())
        if (Cryptographic::CalculateSha256(RoutesDirPath + action->text() + ".json") == currentHash)
            action->setChecked(true);
        else
            action->setChecked(false);
}

void RouteManager::LoadRoutes() {
    QDir routesDir(RoutesDirPath);

    if (!routesDir.exists()) {
        qWarning() << "Directory 'Routes' does not exist.";
        exit(1);
    }

    routesDir.setNameFilters(RoutesFilter);
    QFileInfoList fileList = routesDir.entryInfoList(QDir::Files, QDir::Name);

    if (fileList.isEmpty()) {
        qWarning() << "No .json files found in 'Routes' directory.";
        exit(1);
    }

    RoutesMenu->clear();
    for (const QFileInfo &fileInfo: fileList) {
        auto routeAction = new QAction(fileInfo.fileName().remove(".json"), RoutesMenu);
        routeAction->setCheckable(true);
        RoutesMenu->addAction(routeAction);

        bool connectionSuccess = QObject::connect(routeAction, &QAction::triggered, [fileInfo, this]() {
            if (ConfigFile->exists() && !ConfigFile->remove())
                qWarning() << "Failed to delete" << ConfigFile->fileName();

            if (QFile::copy(fileInfo.absoluteFilePath(), ConfigFile->fileName())) {
                emit routeChanged();
                UpdateActiveRoute();
            } else {
                qWarning() << "Failed to copy" << fileInfo.fileName();
            }
        });

        if (!connectionSuccess) {
            qWarning() << "The signal could not be connected to the slot!";
            delete routeAction;
        }
    }
}

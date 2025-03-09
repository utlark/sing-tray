#include "RouteManager.h"

RouteManager::RouteManager(QObject *parent, QMenu *menu, QFile *configFile) : QObject(parent), routesMenu(menu), configFile(configFile) {
    LoadRoutes();

    if (!configFile->exists() && !routesMenu->actions().isEmpty())
        routesMenu->actions().first()->trigger();

    UpdateActiveRoute();
}

void RouteManager::UpdateActiveRoute() {
    QString currentHash;
    if (configFile->exists())
        currentHash = Cryptographic::CalculateSha256(configFile->fileName());

    for (QAction *action: routesMenu->actions())
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

    routesDir.setNameFilters(routesFilter);
    QFileInfoList fileList = routesDir.entryInfoList(QDir::Files, QDir::Name);

    if (fileList.isEmpty()) {
        qWarning() << "No .json files found in 'Routes' directory.";
        exit(1);
    }

    routesMenu->clear();
    for (const QFileInfo &fileInfo: fileList) {
        auto routeAction = new QAction(fileInfo.fileName().remove(".json"), routesMenu);
        routeAction->setCheckable(true);
        routesMenu->addAction(routeAction);

        bool connectionSuccess = QObject::connect(routeAction, &QAction::triggered, [fileInfo, this]() {
            if (configFile->exists() && !configFile->remove())
                qWarning() << "Failed to delete" << configFile->fileName();

            if (QFile::copy(fileInfo.absoluteFilePath(), configFile->fileName())) {
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

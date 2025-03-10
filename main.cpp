#include <QSystemTrayIcon>
#include <QApplication>
#include <QAction>
#include <QTimer>
#include <QMenu>

#include "Extension/RouteManager.h"
#include "Extension/DottedIcon.h"

#include "Sys/CoreService.h"
#include "Sys/AutoRun.h"

QString appVersion = "v2.1.1";

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CoreService coreService(&app);

    QString iconPath(QCoreApplication::applicationDirPath() + "/Icon.png");

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(DottedIcon::GetIcon(iconPath, Qt::transparent));
    trayIcon.setToolTip("Tray sing-box control center");

    QMenu menu;

    QMenu routingMenu("Active Routing", &menu);
    RouteManager routeManager(&app, &routingMenu, coreService.ConfigFilePath);
    bool connectionSuccess = QObject::connect(&routeManager, &RouteManager::routeChanged, &coreService, &CoreService::Restart);

    QAction restartProxy("Restart Proxy", &menu);
    connectionSuccess = connectionSuccess && QObject::connect(&restartProxy, &QAction::triggered, &coreService, &CoreService::Restart);

    QAction startProxy(coreService.IsRunning() ? "Stop Proxy" : "Start Proxy", &menu);
    connectionSuccess = connectionSuccess && QObject::connect(&startProxy, &QAction::triggered, [&coreService]() {
        if (coreService.IsRunning())
            coreService.Stop();
        else
            coreService.Start();
    });

    QAction startWithSystem("Start with system", &menu);
    startWithSystem.setCheckable(true);
    startWithSystem.setChecked(AutoRun::IsEnabled());
    connectionSuccess = connectionSuccess && QObject::connect(&startWithSystem, &QAction::triggered, AutoRun::SetEnabled);

    QMenu versionMenu("Version", &menu);
    QAction versionAction(appVersion, &versionMenu);
    versionMenu.addAction(&versionAction);

    QAction exitAction("Exit", &menu);
    connectionSuccess = connectionSuccess && QObject::connect(&exitAction, &QAction::triggered, &app, &QApplication::quit);

    QTimer timer;
    QIcon workIcon = DottedIcon::GetIcon(iconPath, Qt::blue);
    QIcon stopIcon = DottedIcon::GetIcon(iconPath, Qt::red);
    connectionSuccess = connectionSuccess && QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (coreService.IsRunning()) {
            trayIcon.setIcon(workIcon);
            startProxy.setText("Stop Proxy");
        } else {
            trayIcon.setIcon(stopIcon);
            startProxy.setText("Start Proxy");
        }
        routeManager.UpdateActiveRoute();
    });

    if (!connectionSuccess)
        qWarning() << "The signal could not be connected to the slot!";

    menu.addMenu(&routingMenu);
    menu.addAction(&restartProxy);
    menu.addAction(&startProxy);
    menu.addSeparator();
    menu.addAction(&startWithSystem);
    menu.addMenu(&versionMenu);
    menu.addAction(&exitAction);

    trayIcon.setContextMenu(&menu);
    timer.start(1000);
    trayIcon.show();

    if (routeManager.ConfigFile.exists())
        coreService.Start();

    return QApplication::exec();
}

#include <QSystemTrayIcon>
#include <QApplication>
#include <QAction>
#include <QTimer>
#include <QMenu>

#include "Extension/RouteManager.h"
#include "Extension/DottedIcon.h"
#include "Resources/Icon.cpp"
#include "Sys/SystemCtl.h"
#include "Sys/AutoRun.h"

QString appVersion = "v1.0.0";

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DottedIcon dottedIcon(":/nekobox.png");
    auto workIcon = dottedIcon.GetIcon(Qt::blue);
    auto stopIcon = dottedIcon.GetIcon(Qt::red);

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(SystemCtl::CheckStatus("sing-box") ? workIcon : stopIcon);
    trayIcon.setToolTip("Tray sing-box control center");

    QMenu menu;

    auto *startWithSystem = new QAction("Start with system", &menu);
    startWithSystem->setCheckable(true);
    startWithSystem->setChecked(AutoRun::IsEnabled());

    auto *startProxy = new QAction(SystemCtl::CheckStatus("sing-box") ? "Stop Proxy" : "Start Proxy", &menu);
    auto *restartProxy = new QAction("Restart Proxy", &menu);

    auto *versionMenu = new QMenu("Version", &menu);
    versionMenu->addAction(new QAction(appVersion, versionMenu));

    auto *exitAction = new QAction("Exit", &menu);

    auto *routingMenu = new QMenu("Active Routing", &menu);
    RouteManager routeManager(routingMenu);

    bool connectionSuccess = QObject::connect(startWithSystem, &QAction::triggered, [=](bool checked) {
        AutoRun::SetEnabled(checked);
    });

    connectionSuccess = connectionSuccess && QObject::connect(startProxy, &QAction::triggered, []() {
        SystemCtl::Execute("sing-box", SystemCtl::CheckStatus("sing-box") ? "stop" : "start");
    });
    connectionSuccess = connectionSuccess && QObject::connect(restartProxy, &QAction::triggered, []() {
        SystemCtl::Execute("sing-box", "restart");
    });

    connectionSuccess = connectionSuccess && QObject::connect(exitAction, &QAction::triggered, &app, &QApplication::quit);

    QTimer timer;
    connectionSuccess = connectionSuccess && QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (SystemCtl::CheckStatus("sing-box")) {
            trayIcon.setIcon(workIcon);
            routeManager.UpdateActiveRoute();
            startProxy->setText("Stop Proxy");
        } else {
            trayIcon.setIcon(stopIcon);
            startProxy->setText("Start Proxy");
        }
    });

    if (!connectionSuccess)
        qCritical() << "The signal could not be connected to the slot!";

    menu.addMenu(routingMenu);
    menu.addAction(restartProxy);
    menu.addAction(startProxy);
    menu.addSeparator();
    menu.addAction(startWithSystem);
    menu.addMenu(versionMenu);
    menu.addAction(exitAction);

    trayIcon.setContextMenu(&menu);
    timer.start(1000);
    trayIcon.show();

    return QApplication::exec();
}

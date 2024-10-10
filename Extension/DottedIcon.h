#ifndef SING_TRAY_DOTTEDICON_H
#define SING_TRAY_DOTTEDICON_H

#pragma once

#include <QIcon>
#include <QColor>
#include <QPixmap>
#include <QPainter>

class DottedIcon {
public:
    explicit DottedIcon(const QString &iconPath);

    QIcon GetIcon(const QColor &dotColor);

private:
    QPixmap basePixmap;
    int radius = 40;
    int x;
    int y;
};

#endif // SING_TRAY_DOTTEDICON_H

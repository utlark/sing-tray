#ifndef SING_TRAY_DOTTEDICON_H
#define SING_TRAY_DOTTEDICON_H

#pragma once

#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QIcon>

class DottedIcon {
public:
    static QIcon GetIcon(QString &iconPath, const QColor &dotColor, int radius = 40, int rightBottomCornerMargin = 16);
};

#endif // SING_TRAY_DOTTEDICON_H

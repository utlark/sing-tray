#include "DottedIcon.h"

DottedIcon::DottedIcon(const QString &iconPath) : basePixmap(iconPath) {
    x = basePixmap.width() - radius - 16;
    y = basePixmap.height() - radius - 16;
}

QIcon DottedIcon::GetIcon(const QColor &dotColor) {
    QPixmap pixmap = basePixmap.copy();

    QPainter painter(&pixmap);
    painter.setBrush(dotColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPoint(x, y), radius, radius);
    painter.end();

    return {pixmap};
}

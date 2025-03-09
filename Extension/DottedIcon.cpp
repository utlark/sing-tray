#include "DottedIcon.h"

QIcon DottedIcon::GetIcon(QString &iconPath, const QColor &dotColor, int radius, int rightBottomCornerMargin) {
    QPixmap pixmap(iconPath);

    QPainter painter(&pixmap);
    painter.setBrush(dotColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPoint(pixmap.width() - radius - rightBottomCornerMargin, pixmap.height() - radius - rightBottomCornerMargin), radius, radius);
    painter.end();

    return pixmap;
}

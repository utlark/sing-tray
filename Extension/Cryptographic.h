#ifndef SING_TRAY_CRYPTOGRAPHIC_H
#define SING_TRAY_CRYPTOGRAPHIC_H

#include <QCryptographicHash>
#include <QDir>

class Cryptographic {
public:
    static QString CalculateSha256(const QString &filePath);
};

#endif //SING_TRAY_CRYPTOGRAPHIC_H

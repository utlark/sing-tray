#include "Cryptographic.h"

QString Cryptographic::CalculateSha256(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file:" << filePath;
        return {};
    }
    return QString::fromLatin1(QCryptographicHash::hash(file.readAll(), QCryptographicHash::RealSha3_256).toHex());
}

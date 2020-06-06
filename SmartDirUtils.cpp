//
// Created by septemberhx on 2020/6/5.
//

#include "SmartDirUtils.h"
#include <QDir>
#include <QDateTime>
#include <QMimeType>
#include <QIcon>
#include <QFileIconProvider>
#include <QDebug>
#include <QPixmap>

QCache<QString, QPixmap> SmartDirUtils::pixmapCache(100);
QMimeDatabase SmartDirUtils::db;

QFileInfoList SmartDirUtils::fileInfoList(const QStringList& pathList) {
    QFileInfoList infoList;

    for (const QString& dirPath : pathList) {
        const QFileInfo dirInfo(dirPath);
        if (dirInfo.exists() && dirInfo.isDir()) {
            const QDir& dir(dirPath);
            infoList << dir.entryInfoList(QDir::Files);
        }
    }

    qSort(infoList.begin(), infoList.end(), [](const QFileInfo& info1, const QFileInfo& info2) {
        return info1.fileTime(QFileDevice::FileModificationTime) > info2.fileTime(QFileDevice::FileModificationTime);
    });
    return infoList;
}

const QPixmap& SmartDirUtils::getFileIcon(const QFileInfo &fileInfo, int w, int h) {
    // I don't know why, but QPixmapCache will be clearly each time
    // So I use QCache instead
    QString key = QString("%1_%2").arg(fileInfo.absoluteFilePath(), QString::number(fileInfo.lastModified().toMSecsSinceEpoch()));
    if (!pixmapCache.contains(key)) {
        QMimeType mime = db.mimeTypeForFile(fileInfo);
        if (mime.name().startsWith("image")) {
            auto *pixmap = new QPixmap(fileInfo.absoluteFilePath());
            *pixmap = pixmap->scaled(w, h, Qt::KeepAspectRatio);
            pixmapCache.insert(key, pixmap);
        } else {
            QIcon icon = QFileIconProvider().icon(fileInfo);
            auto *pixmap = new QPixmap(icon.pixmap(w, h));
            pixmapCache.insert(key, pixmap);
        }
    }
    return *pixmapCache[key];
}

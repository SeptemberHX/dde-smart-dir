//
// Created by septemberhx on 2020/6/5.
//

#include "SmartDirUtils.h"
#include <QDir>
#include <QDateTime>

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

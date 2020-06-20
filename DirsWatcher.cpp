//
// Created by septemberhx on 2020/6/16.
//

#include "DirsWatcher.h"
#include "SmartDirSettings.h"
#include <QDir>
#include <QDateTime>
#include <QDebug>

DirsWatcher::DirsWatcher(QObject *parent) : QObject(parent) {
    this->m_watcher = new QFileSystemWatcher(this);
    connect(this->m_watcher, &QFileSystemWatcher::directoryChanged, this, &DirsWatcher::watchedDirChanged);

    this->reload();
}

void DirsWatcher::reload() {
    this->m_watcher->deleteLater();
    this->fileInfoMap.clear();
    this->m_watcher = new QFileSystemWatcher(this);
    connect(this->m_watcher, &QFileSystemWatcher::directoryChanged, this, &DirsWatcher::watchedDirChanged);

    QStringList stringList = SmartDirSettings::instance()->watchedDirPaths();

    QMap<QString, bool> subFlagMap = SmartDirSettings::instance()->getEnableDirFlagMap();
    for (const QString& dirPath : stringList) {
        QDir dir(dirPath);
        if (dir.exists()) {
            if (subFlagMap.contains(dirPath) && subFlagMap[dirPath]) {
                this->watchDirRecursively(dirPath);
            } else {
                this->m_watcher->addPath(dirPath);
            }
        }
    }

    for (const QString& dirPath : this->m_watcher->directories()) {
        QDir dir(dirPath);
        if (dir.exists()) {
            QFileInfoList infoList;
            for (const QString& subFilePath : dir.entryList(QDir::Files)) {
                infoList << QFileInfo(dir.filePath(subFilePath));
            }
            this->fileInfoMap.insert(dirPath, infoList);
        }
    }

    this->infoChangedOperation();
}

void DirsWatcher::watchDirRecursively(const QString& dirPath) {
    QDir dir(dirPath);
    if (dir.exists()) {
        this->m_watcher->addPath(dirPath);
        for (const QString& subDirPath : dir.entryList(QDir::Dirs)) {
            if (subDirPath == "." || subDirPath == "..") {
                continue;
            }
            this->watchDirRecursively(dir.filePath(subDirPath));
        }
    }
}

void DirsWatcher::watchedDirChanged(const QString &dirPath) {
    if (this->removeFailedPathSet.contains(dirPath))
        return;

    QDir dir(dirPath);
    if (dir.exists()) {  // sub dir changes or file changes
        QSet<QString> dirPathSet;
        for (const QString& subDirPath : dir.entryList(QDir::Dirs)) {
            if (this->fitered(subDirPath)) continue;

            QString dirAbsPath = dir.absoluteFilePath(subDirPath);
            if (!this->m_watcher->directories().contains(dirAbsPath)) {
                this->buildPathInfo(dirAbsPath, true);
            }
            dirPathSet.insert(dirAbsPath);
        }

        QSet<QString> removedPathSet;
        for (const QString& watchedPath : this->m_watcher->directories()) {
            if (watchedPath.mid(0, watchedPath.lastIndexOf('/')) == dirPath && !dirPathSet.contains(watchedPath)) {
                this->removeDir(watchedPath);
                removedPathSet.insert(watchedPath);
            }
        }

        for (const QString& removedPath : removedPathSet) {
            for (const QString &watchedPath : this->m_watcher->directories()) {
                if (watchedPath.startsWith(removedPath)) {
                    this->removeDir(watchedPath);
                }
            }
        }

        this->buildPathInfo(dirPath, false);
    } else {  // dir moved or removed
        this->removeDir(dirPath);
    }

    this->infoChangedOperation();
}

QFileInfoList DirsWatcher::getAllFileInfo() {
    auto allFileInfoList = this->fileInfoMap.values();
    QFileInfoList allFileInfo;
    for (const QFileInfoList& fileInfoList : allFileInfoList) {
        allFileInfo << fileInfoList;
    }
    return allFileInfo;
}

void DirsWatcher::infoChangedOperation() {
    QFileInfoList allFileInfo = this->getAllFileInfo();
    qSort(allFileInfo.begin(), allFileInfo.end(), [](const QFileInfo& info1, const QFileInfo& info2) {
        return info1.lastModified() > info2.lastModified();
    });

//    qDebug() << "=============================== Start ==============================";
//    for (const auto & dirPath : this->m_watcher->directories()) {
//        qDebug() << dirPath;
//    }
//    qDebug() << "Directory" << this->m_watcher->directories().size();
//
//    for (const auto & fileInfo : allFileInfo) {
//        qDebug() << fileInfo.absoluteFilePath();
//    }
//    qDebug() << "Files" << allFileInfo.size();
//    qDebug() << "=============================== End ==============================";

    emit fileInfoListUpdated(allFileInfo.mid(0, this->maxSignalListLength));
}

void DirsWatcher::buildPathInfo(QString dirPath, bool reFlag) {
    QDir dir(dirPath);
    if (dir.exists()) {
        this->m_watcher->addPath(dirPath);

        if (reFlag) {
            for (const QString &subDirPath : dir.entryList(QDir::Dirs)) {
                if (this->fitered(subDirPath)) {
                    continue;
                }
                this->buildPathInfo(dir.absoluteFilePath(subDirPath), true);
            }
        }

        QFileInfoList infoList;
        for (const QString& fileName : dir.entryList(QDir::Files)) {
            infoList << QFileInfo(dir.absoluteFilePath(fileName));
        }
        this->fileInfoMap.insert(dirPath, infoList);
    }
}

bool DirsWatcher::fitered(QString dirAbsPath) {
    return dirAbsPath == "." || dirAbsPath == "..";
}

void DirsWatcher::removeDir(const QString &dirPath) {
    if (!this->m_watcher->removePath(dirPath)) {
        this->removeFailedPathSet.insert(dirPath);
    }
    this->fileInfoMap.remove(dirPath);
}

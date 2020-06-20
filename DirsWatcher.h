//
// Created by septemberhx on 2020/6/16.
//

#ifndef DDE_SMART_DIR_DIRSWATCHER_H
#define DDE_SMART_DIR_DIRSWATCHER_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QString>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QSet>

class DirsWatcher : public QObject {

    Q_OBJECT

public:
    explicit DirsWatcher(QObject *parent = nullptr);
    void reload();

signals:
    void fileInfoListUpdated(const QList<QFileInfo>& fileInfoList);

private slots:
    void watchedDirChanged(const QString& dirPath);

private:
    void watchDirRecursively(const QString& dirPath);
    QFileInfoList getAllFileInfo();
    void infoChangedOperation();

    void buildPathInfo(QString dirPath, bool reFlag);
    bool fitered(QString dirName);
    void removeDir(const QString& dirPath);

private:
    QMap<QString, QFileInfoList> fileInfoMap;
    QFileSystemWatcher *m_watcher;
    QSet<QString> removeFailedPathSet;
    uint maxSignalListLength = 100;
};

#endif //DDE_SMART_DIR_DIRSWATCHER_H

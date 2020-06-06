//
// Created by septemberhx on 2020/6/4.
//

#ifndef DDE_SMART_DIR_SMARTDIRSETTINGS_H
#define DDE_SMART_DIR_SMARTDIRSETTINGS_H

#include <QStringList>
#include <QObject>

class SmartDirSettings : public QObject {
    Q_OBJECT

public:
    static SmartDirSettings *instance();
    const QStringList &watchedDirPaths() const;

    int getItemSize() const;

    void setItemSize(int itemSize);

    void setDefaultItemSize();

    void setWatchedDirPaths(const QStringList &watchedDirPaths);

    int getCountPerPage() const;

    void setCountPerPage(int countPerPage);

    void setDefaultCountPerPage();

signals:
    void settingsChanged();

private:
    SmartDirSettings();
    void saveSettings();
    void readSettings();

private:
    QStringList _watchedDirPaths;
    int itemSize;
    int countPerPage;
};


#endif //DDE_SMART_DIR_SMARTDIRSETTINGS_H

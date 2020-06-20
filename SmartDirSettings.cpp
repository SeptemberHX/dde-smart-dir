//
// Created by septemberhx on 2020/6/4.
//

#include "SmartDirSettings.h"
#include <QSettings>

SmartDirSettings *SmartDirSettings::instance() {
    static auto *inst = new SmartDirSettings();
    return inst;
}

SmartDirSettings::SmartDirSettings() {
    this->_watchedDirPaths << "/home/septemberhx/Downloads" << "/home/septemberhx/Pictures";
    this->setDefaultItemSize();
    this->setDefaultCountPerPage();
    this->setDefaultDirCountPerRow();
    this->setDefaultEnableDirList();
    this->readSettings();

    connect(this, &SmartDirSettings::settingsChanged, this, &SmartDirSettings::saveSettings);
}

const QStringList &SmartDirSettings::watchedDirPaths() const {
    return _watchedDirPaths;
}

int SmartDirSettings::getItemSize() const {
    return itemSize;
}

void SmartDirSettings::setItemSize(int itemSize) {
    SmartDirSettings::itemSize = itemSize;
    emit settingsChanged();
}

void SmartDirSettings::setDefaultItemSize() {
    this->itemSize = 20;
}

void SmartDirSettings::saveSettings() {
    QSettings settings("dde-smart-dir", "smart-dir");
    settings.setValue("main/dirs", this->_watchedDirPaths);
    settings.setValue("main/size", this->itemSize);
    settings.setValue("main/countPerPage", this->countPerPage);
    settings.setValue("main/dirCountPerRow", this->dirCountPerRow);
    settings.setValue("main/enableDirList", this->enableDirList);

    QMap<QString, QVariant> dirFlagMap;
    for (const QString& path : this->enableDirFlagMap.keys()) {
        dirFlagMap.insert(path, this->enableDirFlagMap[path]);
    }
    settings.setValue("main/enableDirFlagMap", dirFlagMap);
}

void SmartDirSettings::readSettings() {
    QSettings settings("dde-smart-dir", "smart-dir");
    this->_watchedDirPaths = settings.value("main/dirs", this->watchedDirPaths()).value<QStringList>();
    this->itemSize = settings.value("main/size", this->itemSize).toUInt();
    this->countPerPage = settings.value("main/countPerPage", this->countPerPage).toUInt();
    this->dirCountPerRow = settings.value("main/dirCountPerRow", this->dirCountPerRow).toUInt();
    this->enableDirList = settings.value("main/enableDirList", this->enableDirList).toBool();

    auto dirFlagMap = settings.value("main/enableDirList", QMap<QString, QVariant>()).toMap();
    for (const QString& path : dirFlagMap.keys()) {
        this->enableDirFlagMap.insert(path, dirFlagMap[path].toBool());
    }
}

void SmartDirSettings::setWatchedDirPaths(const QStringList &watchedDirPaths) {
    _watchedDirPaths = watchedDirPaths;
    emit settingsChanged();
}

int SmartDirSettings::getCountPerPage() const {
    return countPerPage;
}

void SmartDirSettings::setCountPerPage(int countPerPage) {
    SmartDirSettings::countPerPage = countPerPage;
    emit settingsChanged();
}

void SmartDirSettings::setDefaultCountPerPage() {
    this->countPerPage = 5;
}

void SmartDirSettings::setDefaultDirCountPerRow() {
    this->dirCountPerRow = 3;
}

int SmartDirSettings::getDirCountPerRow() const {
    return dirCountPerRow;
}

void SmartDirSettings::setDirCountPerRow(int dirCountPerRow) {
    SmartDirSettings::dirCountPerRow = dirCountPerRow;
    emit settingsChanged();
}

void SmartDirSettings::setDefaultEnableDirList() {
    this->enableDirList = true;
}

bool SmartDirSettings::isEnableDirList() const {
    return enableDirList;
}

void SmartDirSettings::setEnableDirList(bool enableDirList) {
    SmartDirSettings::enableDirList = enableDirList;
    emit settingsChanged();
}

const QMap<QString, bool> &SmartDirSettings::getEnableDirFlagMap() const {
    return enableDirFlagMap;
}

void SmartDirSettings::setEnableDirFlagMap(const QMap<QString, bool> &enableDirFlagMap) {
    SmartDirSettings::enableDirFlagMap = enableDirFlagMap;
    emit settingsChanged();
}

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
}

void SmartDirSettings::readSettings() {
    QSettings settings("dde-smart-dir", "smart-dir");
    this->_watchedDirPaths = settings.value("main/dirs", this->watchedDirPaths()).value<QStringList>();
    this->itemSize = settings.value("main/size", this->itemSize).toUInt();
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
}

void SmartDirSettings::setDefaultCountPerPage() {
    this->countPerPage = 5;
}

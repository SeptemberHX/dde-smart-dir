//
// Created by septemberhx on 2020/6/6.
//

#include "SmartDirPlugin.h"
#include "SmartDirSettings.h"
#include <QApplication>
#include <QDesktopWidget>

#define PLUGIN_STATE_KEY "enable"

SmartDirPlugin::SmartDirPlugin(QObject *parent) : QObject(parent) {

}

const QString SmartDirPlugin::pluginName() const {
    return "dde-smart-dir";
}

void SmartDirPlugin::init(PluginProxyInterface *proxyInter) {
    this->m_proxyInter = proxyInter;

    this->m_smartDirWidget = new SmartDirWidget();
    this->m_smartDirWidget->applySettings(*SmartDirSettings::instance());
    this->m_pluginWidget  = new SmartDirPluginWidget();
    this->m_settingWidget = new SmartDirSettingWidget();

    connect(this->m_settingWidget, &SmartDirSettingWidget::closed, this->m_smartDirWidget, &SmartDirWidget::reloadData);

    if (!pluginIsDisable()) {
        this->m_proxyInter->itemAdded(this, this->pluginName());
    }
}

QWidget *SmartDirPlugin::itemWidget(const QString &itemKey) {
    return this->m_pluginWidget;
}

QWidget *SmartDirPlugin::itemPopupApplet(const QString &itemKey) {
    return this->m_smartDirWidget;
}

bool SmartDirPlugin::pluginIsAllowDisable() {
    return true;
}

bool SmartDirPlugin::pluginIsDisable() {
    return !(m_proxyInter->getValue(this, PLUGIN_STATE_KEY, true).toBool());
}

void SmartDirPlugin::pluginStateSwitched() {
    m_proxyInter->saveValue(this, PLUGIN_STATE_KEY, pluginIsDisable());

    if (pluginIsDisable()) {
        m_proxyInter->itemRemoved(this, pluginName());
        return;
    }

    if (m_pluginWidget) {
        m_proxyInter->itemAdded(this, pluginName());
    }
}

const QString SmartDirPlugin::pluginDisplayName() const {
    return tr("DDE Smart Dir");
}

void SmartDirPlugin::pluginSettingsChanged() {
    if (pluginIsDisable()) {
        m_proxyInter->itemRemoved(this, pluginName());
        return;
    }

    if (m_pluginWidget) {
        m_proxyInter->itemAdded(this, pluginName());
    }
}

int SmartDirPlugin::itemSortKey(const QString &itemKey) {
    Q_UNUSED(itemKey)

    const QString key = QString("pos_%1").arg(Dock::Efficient);
    return m_proxyInter->getValue(this, key, 5).toInt();
}

void SmartDirPlugin::setSortKey(const QString &itemKey, const int order) {
    Q_UNUSED(itemKey)

    const QString key = QString("pos_%1").arg(Dock::Efficient);
    m_proxyInter->saveValue(this, key, order);
}

const QString SmartDirPlugin::itemContextMenu(const QString &itemKey) {
    Q_UNUSED(itemKey)

    QList<QVariant> items;
    items.reserve(1);

    QMap<QString, QVariant> open;
    open["itemId"] = "settings";
    open["itemText"] = tr("Settings");
    open["isActive"] = true;
    items.push_back(open);

    QMap<QString, QVariant> menu;
    menu["items"] = items;
    menu["checkableMenu"] = false;
    menu["singleCheck"] = false;

    // 返回 JSON 格式的菜单数据
    return QJsonDocument::fromVariant(menu).toJson();
}

void SmartDirPlugin::invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked) {
    Q_UNUSED(itemKey)

    // 根据上面接口设置的 id 执行不同的操作
    if (menuId == "settings") {
        this->m_settingWidget->loadData();
        int screenNum = QApplication::desktop()->screenNumber(this->m_smartDirWidget);
        this->m_settingWidget->move(QApplication::desktop()->screen(screenNum)->rect().center() - this->m_settingWidget->rect().center());
        this->m_settingWidget->show();
        this->m_settingWidget->show();
    }
}

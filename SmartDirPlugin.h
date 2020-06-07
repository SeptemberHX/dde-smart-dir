//
// Created by septemberhx on 2020/6/6.
//

#ifndef DDE_SMART_DIR_SMARTDIRPLUGIN_H
#define DDE_SMART_DIR_SMARTDIRPLUGIN_H

#include <QObject>
#include <dde-dock/pluginsiteminterface.h>
#include <QFileSystemWatcher>
#include <QTimer>
#include "widgets/smartdirwidget.h"
#include "SmartDirPluginWidget.h"
#include "widgets/fanlistwidget.h"
#include "widgets/smartdirsettingwidget.h"

class SmartDirPlugin : public QObject, PluginsItemInterface {

    Q_OBJECT

    Q_INTERFACES(PluginsItemInterface)

    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "dde_smart_dir.json")

public:
    explicit SmartDirPlugin(QObject *parent = nullptr);

    // 返回插件的名称，必须是唯一值，不可以和其它插件冲突
    const QString pluginName() const override;

    // 插件初始化函数
    void init(PluginProxyInterface *proxyInter) override;

    // 返回插件的 widget
    QWidget *itemWidget(const QString &itemKey) override;

//    QWidget *itemPopupApplet(const QString &itemKey) override;

    bool pluginIsAllowDisable() override;
    bool pluginIsDisable() override;
    void pluginStateSwitched() override;
    const QString pluginDisplayName() const override;
    void pluginSettingsChanged() override;

    int itemSortKey(const QString &itemKey) Q_DECL_OVERRIDE;
    void setSortKey(const QString &itemKey, const int order) Q_DECL_OVERRIDE;

    const QString itemContextMenu(const QString &itemKey) override;
    void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked) override;

private:
    void reloadData();
    void loadData(const QFileInfoList& infoList);

    SmartDirWidget *m_smartDirWidget;
    SmartDirPluginWidget *m_pluginWidget;
    SmartDirSettingWidget *m_settingWidget;

    QFileSystemWatcher *m_dirWatcher;
    QTimer *m_reloadTimer;
    FanListWidget *m_fanListWidget;
};


#endif //DDE_SMART_DIR_SMARTDIRPLUGIN_H

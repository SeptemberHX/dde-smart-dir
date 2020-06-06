#ifndef SMARTDIRWIDGET_H
#define SMARTDIRWIDGET_H

#include <QWidget>
#include <QFileInfoList>
#include <QLabel>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QFileSystemWatcher>
#include "SmartDirSettings.h"

namespace Ui {
class SmartDirWidget;
}


class SmartDirItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SmartDirItemWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    const QFileInfo& fileInfo() const;

private:
    QFileInfo m_fileInfo;
    QLabel *m_iconLabel;
    QLabel *m_nameLabel;
    QHBoxLayout *m_layout;
};

class SmartDirTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit SmartDirTableWidget(QWidget *parent = nullptr);

private:
    void actionTriggered();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QMenu *m_menu;
    QAction *m_openFolderAction;
    QPoint dragStartPosition;
};


class SmartDirWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SmartDirWidget(QWidget *parent = nullptr);
    void loadData(const QFileInfoList& infoList);
    void applySettings(const SmartDirSettings& settings);

    void reloadData();

private slots:
    void doubleClick(const QModelIndex& index);

private:
    SmartDirTableWidget *m_tableWidget;
    QVBoxLayout *m_layout;
    QFileSystemWatcher *m_dirWatcher;
    QTimer *m_reloadTimer;
};

#endif // SMARTDIRWIDGET_H

#ifndef SMARTDIRWIDGET_H
#define SMARTDIRWIDGET_H

#include <QWidget>
#include <QFileInfoList>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidgetItem>
#include <QFileSystemWatcher>
#include <QGroupBox>
#include <QPushButton>
#include "SmartDirSettings.h"

namespace Ui {
class SmartDirWidget;
}


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
    void dirButtonClicked();

private:
    SmartDirTableWidget *m_tableWidget;
    QVBoxLayout *m_layout;
    QFileSystemWatcher *m_dirWatcher;
    QTimer *m_reloadTimer;

    QGroupBox *m_dirBox;
    QGridLayout *m_boxLayout;
    QList<QPushButton*> buttonList;
};

#endif // SMARTDIRWIDGET_H

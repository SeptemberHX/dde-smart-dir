//
// Created by septemberhx on 2020/6/8.
//

#ifndef DDE_SMART_DIR_SMARTDIRITEMWIDGET_H
#define DDE_SMART_DIR_SMARTDIRITEMWIDGET_H

#include <QWidget>
#include <QFileInfo>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "SpecialLabel.h"


class SmartDirItemWidget : public QWidget
{
Q_OBJECT

public:
    explicit SmartDirItemWidget(QWidget *parent = nullptr);
    const QFileInfo& fileInfo() const;
    void setFileInfo(const QFileInfo& fileInfo);
    void refreshIcon();

private:
    QFileInfo m_fileInfo;
    QLabel *m_iconLabel;
    SpecialLabel *m_nameLabel;
    QHBoxLayout *m_layout;
    QVBoxLayout *m_textLayout;
    QWidget *m_textWidget;
    QLabel *m_filePathLabel;
    QLabel *m_fileDataLabel;
};

#endif //DDE_SMART_DIR_SMARTDIRITEMWIDGET_H

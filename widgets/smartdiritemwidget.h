//
// Created by septemberhx on 2020/6/7.
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
    explicit SmartDirItemWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    const QFileInfo& fileInfo() const;
    void setFileInfo(const QFileInfo& fileInfo);

signals:
    void doubleClicked(const QFileInfo& fileInfo);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QFileInfo m_fileInfo;
    QLabel *m_iconLabel;
//    QLabel *m_nameLabel;
    SpecialLabel *m_nameLabel;
    QHBoxLayout *m_layout;
    QVBoxLayout *m_textLayout;
    QWidget *m_textWidget;
    SpecialLabel *m_filePathLabel;
    SpecialLabel *m_fileDataLabel;
};


#endif //DDE_SMART_DIR_SMARTDIRITEMWIDGET_H

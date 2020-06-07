//
// Created by septemberhx on 2020/6/6.
//

#ifndef DDE_SMART_DIR_SMARTDIRPLUGINWIDGET_H
#define DDE_SMART_DIR_SMARTDIRPLUGINWIDGET_H

#include <QLabel>
#include <QIcon>

class SmartDirPluginWidget : public QLabel {

    Q_OBJECT

public:
    explicit SmartDirPluginWidget(QWidget *parent = nullptr);
    void updateIcon();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *ev) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_icon;
    QIcon m_defaultIcon;
};


#endif //DDE_SMART_DIR_SMARTDIRPLUGINWIDGET_H

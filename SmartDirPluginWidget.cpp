//
// Created by septemberhx on 2020/6/6.
//

#include "SmartDirPluginWidget.h"

#include <constants.h>
#include <QPainter>
#include <QMouseEvent>


SmartDirPluginWidget::SmartDirPluginWidget(QWidget *parent)
    : QLabel(parent)
{
    this->setMinimumSize(PLUGIN_BACKGROUND_MIN_SIZE, PLUGIN_BACKGROUND_MIN_SIZE);
    this->m_defaultIcon = QIcon(":/icons/folder.svg");
}

void SmartDirPluginWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    this->updateIcon();

    QPainter painter(this);
    const QRectF &rf = QRectF(rect());
    const QRectF &rfp = QRectF(this->m_icon.rect());
    painter.drawPixmap(rf.center() - rfp.center() / this->devicePixelRatioF(), this->m_icon);
}

void SmartDirPluginWidget::updateIcon() {
    int size = std::min(width(), height());
    if (size < PLUGIN_ICON_MIN_SIZE)
        size = PLUGIN_ICON_MIN_SIZE;
    if (size > PLUGIN_BACKGROUND_MAX_SIZE) {
        size *= ((Dock::Fashion == qApp->property(PROP_DISPLAY_MODE).value<Dock::DisplayMode>()) ? 0.8 : 0.7);
        if (size < PLUGIN_BACKGROUND_MAX_SIZE)
            size = PLUGIN_BACKGROUND_MAX_SIZE;
    }

    QIcon icon = QIcon::fromTheme( "folder.svg", this->m_defaultIcon);

    const auto ratio = devicePixelRatioF();
    this->m_icon = icon.pixmap(size * ratio, size * ratio);
    this->m_icon.setDevicePixelRatio(ratio);
}

void SmartDirPluginWidget::mousePressEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        emit clicked();
    }
    QLabel::mousePressEvent(ev);
}

//
// Created by septemberhx on 2020/6/7.
//

#include "SpecialLabel.h"
#include <QPaintEvent>
#include <QPainter>
#include <QFontMetrics>

SpecialLabel::SpecialLabel(QWidget *parent,  QColor bgC, QColor fC)
    : QLabel(parent)
    , bgColor(bgC)
    , fontColor(fC)
{

}

void SpecialLabel::paintEvent(QPaintEvent *event) {
    QFontMetrics fontMetrics(this->font());
    QRect rect = fontMetrics.boundingRect(this->text());
    QString textStr = this->text();
    if (rect.width() > event->rect().width() - radius) {
         textStr = fontMetrics.elidedText(this->text(), Qt::ElideMiddle, event->rect().width() - radius);
    }
    rect = fontMetrics.boundingRect(textStr);
    rect.moveTo(this->rect().topLeft() + QPoint(0, (this->rect().height() - rect.height()) / 2));
    rect.setWidth(qMin(rect.width() + radius, event->rect().width()));

    QPainter p(this);
    p.setRenderHint(QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    QPen pen(this->bgColor, 1);
    p.setPen(pen);
    p.fillPath(path, this->bgColor);
    p.drawPath(path);

    p.setPen(this->fontColor);
    rect.moveLeft(this->rect().left() + radius / 2);
    p.drawText(rect, textStr);
}

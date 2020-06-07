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
    if (rect.width() > event->rect().width() - radius * 2) {
         textStr = fontMetrics.elidedText(this->text(), Qt::ElideMiddle, event->rect().width() - radius * 2);
    }
    rect = fontMetrics.boundingRect(textStr);
    rect.moveTo(this->rect().topLeft() + QPoint(0, (this->rect().height() - rect.height()) / 2));
    rect.setWidth(rect.width() + 2 * radius);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    QPen pen(Qt::black, 1);
    p.setPen(pen);
    p.fillPath(path, this->bgColor);
    p.drawPath(path);

    p.setPen(this->fontColor);
    rect.moveLeft(this->rect().left() + radius);
    p.drawText(rect, textStr);
}

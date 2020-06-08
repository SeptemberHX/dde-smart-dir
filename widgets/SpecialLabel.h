//
// Created by septemberhx on 2020/6/7.
//

#ifndef DDE_SMART_DIR_SPECIALLABEL_H
#define DDE_SMART_DIR_SPECIALLABEL_H


#include <QLabel>
#include <QColor>


class SpecialLabel : public QLabel {

    Q_OBJECT

public:
    explicit SpecialLabel(QWidget *parent = nullptr, QColor bgC = Qt::gray, QColor fC = Qt::white);

protected:
    void paintEvent(QPaintEvent *event) override;
    int radius = 10;
    QColor bgColor;
    QColor fontColor;
};


#endif //DDE_SMART_DIR_SPECIALLABEL_H

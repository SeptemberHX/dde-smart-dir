#ifndef FANLISTWIDGET_H
#define FANLISTWIDGET_H

#include <QGraphicsView>
#include <QFileInfo>
#include <QPropertyAnimation>
#include "smartdiritemwidget.h"


class FanListWidget : public QGraphicsView
{
    Q_OBJECT

public:
    FanListWidget(QWidget *parent = nullptr);
    void resetWidget(QList<QFileInfo> infoList, int w, int h, int r);
    void slowHide();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    QList<SmartDirItemWidget*> widgetList;
    QGraphicsScene *m_scene;
    QPropertyAnimation *m_buttonShowAnimation;
    QPropertyAnimation *m_buttonHideAnimation;
    QPoint dragStartPosition;
    QTimer *m_hideTimer;

    QPoint calcOffset(int w, int h, int r, int i, double *angle);
    void calcGeo(int w, int h, int r, int n, int *W, int *H, int *x0);
};

#endif // FANLISTWIDGET_H

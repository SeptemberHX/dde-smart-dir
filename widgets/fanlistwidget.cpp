#include "fanlistwidget.h"
#include <cmath>
#include <QDebug>
#include <QDrag>
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QGraphicsProxyWidget>
#include <QDesktopServices>
#include <QTimer>
#include <QDir>
#include "SmartDirSettings.h"

#define PI 3.141592657
#define XE 10
#define YE 10

FanListWidget::FanListWidget(QWidget *parent)
    : QGraphicsView (parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);
    this->setStyleSheet("background-color: rgba(0,0,0,0)");
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->m_scene = new QGraphicsScene(this);
    this->setScene(this->m_scene);
    this->setMouseTracking(true);
    this->m_hideTimer = new QTimer(this);
    this->m_hideTimer->setSingleShot(true);
    this->m_hideTimer->setInterval(100);
    connect(this->m_hideTimer, &QTimer::timeout, this, &FanListWidget::hide);
}

void FanListWidget::resetWidget(QList<QFileInfo> infoList, int w, int h, int r)
{
    int maxItemSize = SmartDirSettings::instance()->getCountPerPage();
    int i;
    for (i = 0; i < this->widgetList.size() && i < infoList.size(); ++i) {
        widgetList[i]->setFileInfo(infoList[i]);
    }
    for (; i < maxItemSize && i < infoList.size(); ++i) {
        auto widget = new SmartDirItemWidget(infoList[i]);
        connect(widget, &SmartDirItemWidget::doubleClicked, this, [this](const QFileInfo& info) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
        });
        widgetList.append(widget);

        QGraphicsProxyWidget* pWidget = m_scene->addWidget(widget);
        proxyWidgetList.append(pWidget);
    }

    int W, H, x0;
    calcGeo(w, h, r, this->widgetList.size(), &W, &H, &x0);
    this->setFixedSize(W, H);

    proxyWidgetList[0]->setRotation(0);
    proxyWidgetList[0]->setPos(x0, 0);
    for (i = 1; i < this->widgetList.size() && i < maxItemSize; ++i) {
        double angle = 0.0;
        QGraphicsProxyWidget *t_pWidget = proxyWidgetList[i];
        QPoint offset = calcOffset(w, h, r, i, &angle);
        t_pWidget->setRotation(angle);
        t_pWidget->setPos(x0 - offset.x(), 0 + offset.y());
    }
}


QPoint FanListWidget::calcOffset(int w, int h, int r, int i, double *angle)
{
    double tanA = h * 1.0 / r;
    double atanA = atan(tanA) * i;
    *angle = atanA * 180 / PI;
    double sinA = sin(atanA);
    double cosA = cos(atanA);

    double offsetX = w * cosA + r * cosA - h * sinA;
    double offsetY = h * cosA + w * sinA + r * sinA - h;
    return QPoint(w + r - offsetX, offsetY);
}

void FanListWidget::calcGeo(int w, int h, int r, int n, int *W, int *H, int *x0)
{
    double tanA = h * 1.0 / r;
    double atanA = atan(tanA) * (n - 1);
    double sinA = sin(atanA);
    double cosA = cos(atanA);

    *W = w + r - r * cosA + h * sinA + XE * n;
    *H = (r + w) * sinA + h * cosA + YE * n;
    *x0 = *W - w;
}

void FanListWidget::focusOutEvent(QFocusEvent *event) {
    this->slowHide();
    QGraphicsView::focusOutEvent(event);
}

void FanListWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    } else {
        QList<QGraphicsItem*> itemList = this->items(event->pos());
        if (itemList.size() != 1)
            return;

        auto proxyWidget = qgraphicsitem_cast<QGraphicsProxyWidget*>(itemList[0]);
        if (proxyWidget != nullptr) {
            auto widget = qobject_cast<SmartDirItemWidget *>(proxyWidget->widget());
            QDesktopServices::openUrl(QUrl::fromLocalFile(widget->fileInfo().absoluteDir().absolutePath()));
        }
    }
    QWidget::mousePressEvent(event);
}

void FanListWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QList<QGraphicsItem*> itemList = this->items(dragStartPosition);
    if (itemList.size() != 1)
        return;

    auto proxyWidget = qgraphicsitem_cast<QGraphicsProxyWidget*>(itemList[0]);
    if (proxyWidget != nullptr) {
        auto widget = qobject_cast<SmartDirItemWidget*>(proxyWidget->widget());
        auto drag = new QDrag(this);
        auto mimeData = new QMimeData;

        QList<QUrl> urlList;
        urlList.append(QUrl::fromLocalFile(widget->fileInfo().absoluteFilePath()));
        mimeData->setUrls(urlList);
        drag->setMimeData(mimeData);
        drag->setPixmap(widget->grab());
        drag->exec(Qt::CopyAction | Qt::MoveAction);
    } else {
        qDebug() << "No dragged item";
    }
}

void FanListWidget::slowHide() {
    this->m_hideTimer->start();
}

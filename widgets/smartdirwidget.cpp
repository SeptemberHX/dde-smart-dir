#include "smartdirwidget.h"
#include <QFileIconProvider>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QHeaderView>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMenu>
#include <QAction>
#include <QDir>
#include "SmartDirUtils.h"
#include <QTimer>

#define ICON_SIZE 64

SmartDirWidget::SmartDirWidget(QWidget *parent) :
    QWidget(parent)
{
    this->m_tableWidget = new SmartDirTableWidget(this);
    this->m_layout = new QVBoxLayout(this);
    this->m_layout->addWidget(this->m_tableWidget);

    setStyleSheet("background-color: rgba(0,0,0,0)");
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFixedSize(400, 800);

    this->m_dirWatcher = new QFileSystemWatcher(this);
    this->m_reloadTimer = new QTimer(this);
    this->m_reloadTimer->setInterval(500);
    this->m_reloadTimer->setSingleShot(true);

    connect(this->m_tableWidget, &QTableWidget::doubleClicked, this, &SmartDirWidget::doubleClick);
    connect(this->m_dirWatcher, &QFileSystemWatcher::directoryChanged, this->m_reloadTimer, qOverload<>(&QTimer::start));
    connect(this->m_reloadTimer, &QTimer::timeout, this, &SmartDirWidget::reloadData);
    this->reloadData();
}

void SmartDirWidget::loadData(const QFileInfoList &infoList)
{
    this->m_tableWidget->clear();
    this->m_tableWidget->setColumnCount(1);
    this->m_tableWidget->setRowCount(0);

    for (int r = 0; r < infoList.size(); ++r) {
        this->m_tableWidget->insertRow(r);
        auto *itemWidget = new SmartDirItemWidget(infoList[r], this->m_tableWidget);
        this->m_tableWidget->setCellWidget(r, 0, itemWidget);
    }

    this->setFixedHeight(10 * this->m_tableWidget->verticalHeader()->defaultSectionSize() + 20);
}

void SmartDirWidget::doubleClick(const QModelIndex &index) {
    auto *itemWidget = dynamic_cast<SmartDirItemWidget*>(this->m_tableWidget->cellWidget(index.row(), 0));
    QDesktopServices::openUrl(QUrl::fromLocalFile(itemWidget->fileInfo().absoluteFilePath()));
}

void SmartDirWidget::applySettings(const SmartDirSettings& settings) {
    this->m_dirWatcher->removePaths(this->m_dirWatcher->directories());
    this->m_dirWatcher->addPaths(settings.watchedDirPaths());
}

void SmartDirWidget::reloadData() {
    this->loadData(SmartDirUtils::fileInfoList(SmartDirSettings::instance()->watchedDirPaths()).mid(0, SmartDirSettings::instance()->getItemSize()));
}

SmartDirItemWidget::SmartDirItemWidget(QFileInfo fileInfo, QWidget *parent)
    : QWidget(parent)
{
    this->m_layout = new QHBoxLayout(this);
    this->m_fileInfo = fileInfo;

    this->setToolTip(fileInfo.absoluteFilePath());

    this->m_iconLabel = new QLabel(this);
//    m_iconLabel->setScaledContents(true);
    m_iconLabel->setFixedSize(ICON_SIZE, ICON_SIZE);
    m_iconLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    QImage image;
    if (image.load(fileInfo.filePath())) {
        m_iconLabel->setPixmap(QPixmap::fromImage(image).scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio));
    } else {
        QIcon icon = QFileIconProvider().icon(fileInfo);
        m_iconLabel->setPixmap(icon.pixmap(icon.actualSize(QSize(ICON_SIZE, ICON_SIZE))));
    }
    this->m_layout->addWidget(this->m_iconLabel);

    this->m_nameLabel = new QLabel(this);
    this->m_nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    this->m_nameLabel->setText(fileInfo.fileName());
    this->m_layout->addWidget(this->m_nameLabel);
    this->m_layout->setContentsMargins(0, 0, 0, 0);
    this->m_nameLabel->setStyleSheet("QLabel { color: black; } ");

    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

const QFileInfo &SmartDirItemWidget::fileInfo() const {
    return m_fileInfo;
}

SmartDirTableWidget::SmartDirTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    this->setShowGrid(false);
    QHeaderView *verticalHeader = this->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(ICON_SIZE + 10);

    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);
    this->horizontalHeader()->setStretchLastSection(true);

    this->m_menu = new QMenu(this);
    this->m_openFolderAction = new QAction(tr("Open contained folder"), this->m_menu);
    connect(this->m_openFolderAction, &QAction::triggered, this, &SmartDirTableWidget::actionTriggered);
    this->m_menu->addAction(this->m_openFolderAction);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTableWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
        this->m_menu->popup(this->mapToGlobal(pos));
    });

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void SmartDirTableWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QTableWidget::mousePressEvent(event);
}

void SmartDirTableWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    auto *itemWidget = dynamic_cast<SmartDirItemWidget*>(this->cellWidget(this->currentRow(), 0));
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    QList<QUrl> urlList;
    urlList.append(QUrl::fromLocalFile(itemWidget->fileInfo().absoluteFilePath()));
    mimeData->setUrls(urlList);
    drag->setMimeData(mimeData);
    drag->setPixmap(itemWidget->grab());
    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void SmartDirTableWidget::actionTriggered() {
    QAction *actionPtr = dynamic_cast<QAction*>(sender());
    if (actionPtr == this->m_openFolderAction) {
        auto *itemWidget = dynamic_cast<SmartDirItemWidget*>(this->cellWidget(this->currentRow(), 0));
        QDesktopServices::openUrl(QUrl(itemWidget->fileInfo().absoluteDir().absolutePath()));
    }
}


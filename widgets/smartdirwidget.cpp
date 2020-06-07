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
#include <QDateTime>

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
    this->m_tableWidget->setColumnCount(1);

    int r;
    for (r = 0; r < infoList.size() && r < this->m_tableWidget->rowCount(); ++r) {
        auto *itemWidget = dynamic_cast<SmartDirItemWidget*>(this->m_tableWidget->cellWidget(r, 0));
        itemWidget->setFileInfo(infoList.at(r));
    }

    for (; r < infoList.size(); ++r) {
        this->m_tableWidget->insertRow(r);
        auto *itemWidget = new SmartDirItemWidget(infoList[r], this->m_tableWidget);
        this->m_tableWidget->setCellWidget(r, 0, itemWidget);
    }
    this->m_tableWidget->setRowCount(infoList.size());

    this->setFixedHeight(SmartDirSettings::instance()->getCountPerPage() * this->m_tableWidget->verticalHeader()->defaultSectionSize() + 10);
}

void SmartDirWidget::doubleClick(const QModelIndex &index) {
    if (index.isValid()) {
        auto *itemWidget = dynamic_cast<SmartDirItemWidget *>(this->m_tableWidget->cellWidget(index.row(), 0));
        QDesktopServices::openUrl(QUrl::fromLocalFile(itemWidget->fileInfo().absoluteFilePath()));
    }
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
    m_iconLabel->setFixedSize(ICON_SIZE, ICON_SIZE);
    m_iconLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    this->m_layout->addWidget(this->m_iconLabel);

    this->m_textWidget = new QWidget(this);
    this->m_layout->addWidget(this->m_textWidget);
    this->m_textLayout = new QVBoxLayout(this->m_textWidget);

    this->m_nameLabel = new QLabel(this->m_textWidget);
    this->m_nameLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    this->m_textLayout->addWidget(this->m_nameLabel);

    this->m_filePathLabel = new QLabel(this->m_textWidget);
    this->m_filePathLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    this->m_textLayout->addWidget(this->m_filePathLabel);

    this->m_fileDataLabel = new QLabel(this->m_textWidget);
    this->m_fileDataLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->m_textLayout->addWidget(this->m_fileDataLabel);

    this->m_textLayout->setContentsMargins(0, 8, 0, 8);
    this->m_textLayout->setSpacing(0);
    this->m_layout->setContentsMargins(0, 0, 0, 0);
    this->m_nameLabel->setStyleSheet("QLabel { color: black; } ");

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFileInfo(fileInfo);
}

const QFileInfo &SmartDirItemWidget::fileInfo() const {
    return m_fileInfo;
}

void SmartDirItemWidget::setFileInfo(const QFileInfo &fileInfo) {
    this->m_fileInfo = fileInfo;

    this->m_iconLabel->setPixmap(SmartDirUtils::getFileIcon(fileInfo, ICON_SIZE, ICON_SIZE));

    QString nameStr = fileInfo.fileName();
    QFontMetrics font(this->m_nameLabel->font());
    int font_size = font.width(nameStr);
    if(font_size > 290){
        nameStr = font.elidedText(nameStr, Qt::ElideMiddle, 290);
    }
    this->m_nameLabel->setText(nameStr);

    QString pathStr = fileInfo.absoluteDir().absolutePath();
    font = QFontMetrics(this->m_filePathLabel->font());
    font_size = font.width(pathStr);
    if(font_size > 290){
        pathStr = font.elidedText(pathStr, Qt::ElideMiddle, 290);
    }
    this->m_filePathLabel->setText(pathStr);

    QString dataStr = fileInfo.lastModified().toString();
    font = QFontMetrics(this->m_fileDataLabel->font());
    font_size = font.width(dataStr);
    if(font_size > 290){
        pathStr = font.elidedText(dataStr, Qt::ElideMiddle, 290);
    }
    this->m_fileDataLabel->setText(dataStr);
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

    if (this->currentRow() < 0)
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


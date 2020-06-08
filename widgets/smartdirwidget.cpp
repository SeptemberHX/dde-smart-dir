#include "smartdirwidget.h"
#include "SmartDirItemWidget.h"
#include "SmartDirUtils.h"
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
#include <QTimer>
#include <QDateTime>
#include <QPushButton>

#define ICON_SIZE 64

SmartDirWidget::SmartDirWidget(QWidget *parent) :
    QWidget(parent)
{
    this->m_layout = new QVBoxLayout(this);

    this->m_dirBox = new QGroupBox(this);
    this->m_dirBox->setTitle(tr("Directory"));
    this->m_boxLayout = new QGridLayout(this->m_dirBox);
    this->m_boxLayout->setContentsMargins(0, 2, 0, 2);
    this->m_layout->addWidget(this->m_dirBox);

    this->m_tableWidget = new SmartDirTableWidget(this);
    this->m_layout->addWidget(this->m_tableWidget);

    setStyleSheet("background-color: rgba(0,0,0,0)");
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFixedWidth(400);

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
    // dir part
    if (SmartDirSettings::instance()->isEnableDirList()) {
        this->m_dirBox->show();
        QStringList dirPathList = SmartDirSettings::instance()->watchedDirPaths();
        int countPerRow = SmartDirSettings::instance()->getDirCountPerRow();
        if (this->m_boxLayout->rowCount() > 1 && this->m_boxLayout->columnCount() < countPerRow || this->m_boxLayout->columnCount() > countPerRow) {
            for (int i = 0; i < this->m_boxLayout->count(); ++i) {
                this->m_boxLayout->removeItem(this->m_boxLayout->itemAt(i));
            }
            delete this->m_boxLayout;
            this->m_boxLayout = new QGridLayout(this->m_dirBox);
            for (int i = 0; i < this->buttonList.size(); ++i) {
                this->m_boxLayout->addWidget(this->buttonList.at(i), i / countPerRow, i % countPerRow);
            }
        }

        int r;
        for (r = 0; r < dirPathList.size() && r < buttonList.size(); ++r) {
            buttonList[r]->setText(QDir(dirPathList.at(r)).dirName());
            buttonList[r]->show();
        }

        int existNum = r;
        if (r < dirPathList.size()) {
            for (; r < dirPathList.size(); ++r) {
                auto *button = new QPushButton(QDir(dirPathList.at(r)).dirName(), this->m_dirBox);
                button->setStyleSheet("QPushButton { color: white; background-color: #454346;}");
                connect(button, &QPushButton::clicked, this, &SmartDirWidget::dirButtonClicked);
                this->m_boxLayout->addWidget(button, r / countPerRow, r % countPerRow);
                this->buttonList.append(button);
            }
        } else {
            for (; r < buttonList.size(); ++r) {
                buttonList.at(r)->hide();
            }
        }
    } else {
        this->m_dirBox->hide();
    }

    // file part
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
        itemWidget->setFixedWidth(390);
    }
    this->m_tableWidget->setRowCount(infoList.size());
    this->m_tableWidget->setFixedHeight(SmartDirSettings::instance()->getCountPerPage() * this->m_tableWidget->verticalHeader()->defaultSectionSize() + 10);
    this->m_tableWidget->update();
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

void SmartDirWidget::dirButtonClicked() {
    auto *button = qobject_cast<QPushButton*>(sender());
    int index = this->buttonList.indexOf(button);
    QString dirPath = SmartDirSettings::instance()->watchedDirPaths().at(index);
    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath));
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
    auto *drag = new QDrag(this);
    auto *mimeData = new QMimeData;

    QList<QUrl> urlList;
    urlList.append(QUrl::fromLocalFile(itemWidget->fileInfo().absoluteFilePath()));
    mimeData->setUrls(urlList);
    drag->setMimeData(mimeData);
    drag->setPixmap(itemWidget->grab());
    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void SmartDirTableWidget::actionTriggered() {
    auto *actionPtr = dynamic_cast<QAction*>(sender());
    if (actionPtr == this->m_openFolderAction) {
        auto *itemWidget = dynamic_cast<SmartDirItemWidget*>(this->cellWidget(this->currentRow(), 0));
        QDesktopServices::openUrl(QUrl(itemWidget->fileInfo().absoluteDir().absolutePath()));
    }
}


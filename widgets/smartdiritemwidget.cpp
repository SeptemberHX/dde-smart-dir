//
// Created by septemberhx on 2020/6/7.
//

#include "smartdiritemwidget.h"
#include <QDir>
#include <QDateTime>
#include "SmartDirUtils.h"
#include <QDebug>
#include <QMouseEvent>

#define ICON_SIZE 60

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

    this->m_nameLabel = new SpecialLabel(this->m_textWidget, QColor("#101010"), Qt::white);
    this->m_textLayout->addWidget(this->m_nameLabel);

    this->m_filePathLabel = new SpecialLabel(this->m_textWidget, Qt::darkGray, Qt::white);
    this->m_textLayout->addWidget(this->m_filePathLabel);

    this->m_fileDataLabel = new SpecialLabel(this->m_textWidget, Qt::darkGray, Qt::white);
    this->m_textLayout->addWidget(this->m_fileDataLabel);

    this->m_textLayout->setContentsMargins(0, 8, 0, 8);
    this->m_textLayout->setSpacing(0);
    this->m_layout->setContentsMargins(0, 0, 0, 0);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFileInfo(fileInfo);
    this->setMouseTracking(true);
    this->m_iconLabel->setMouseTracking(true);
    this->m_textWidget->setMouseTracking(true);
    this->m_nameLabel->setMouseTracking(true);
    this->m_filePathLabel->setMouseTracking(true);
    this->m_fileDataLabel->setMouseTracking(true);
}

const QFileInfo &SmartDirItemWidget::fileInfo() const {
    return m_fileInfo;
}

void SmartDirItemWidget::setFileInfo(const QFileInfo &fileInfo) {
    this->m_fileInfo = fileInfo;

    this->m_iconLabel->setPixmap(SmartDirUtils::getFileIcon(fileInfo, ICON_SIZE, ICON_SIZE));

    QString nameStr = fileInfo.fileName();
    QFontMetrics font(this->m_nameLabel->font());
    this->m_nameLabel->setText(nameStr);

    QString pathStr = fileInfo.absoluteDir().absolutePath();
    this->m_filePathLabel->setText(pathStr);

    QString dataStr = fileInfo.lastModified().toString();
    this->m_fileDataLabel->setText(dataStr);
}

void SmartDirItemWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(this->fileInfo());
    }
    QWidget::mouseDoubleClickEvent(event);
}

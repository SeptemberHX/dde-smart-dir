//
// Created by septemberhx on 2020/6/8.
//

#include "SmartDirItemWidget.h"
#include "SmartDirUtils.h"
#include <QDateTime>
#include <QDir>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

#define ICON_SIZE 60

SmartDirItemWidget::SmartDirItemWidget(QWidget *parent)
        : QWidget(parent)
{
    this->m_layout = new QHBoxLayout(this);

    this->m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(ICON_SIZE, ICON_SIZE);
    m_iconLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    this->m_layout->addWidget(this->m_iconLabel);

    auto *coverShadow = new QGraphicsDropShadowEffect;
    coverShadow->setColor(QPalette::Shadow);
    coverShadow->setBlurRadius(5);
    coverShadow->setOffset(4);
    this->m_iconLabel->setGraphicsEffect(coverShadow);


    this->m_textWidget = new QWidget(this);
    this->m_layout->addWidget(this->m_textWidget);
    this->m_textLayout = new QVBoxLayout(this->m_textWidget);

    this->m_nameLabel = new SpecialLabel(this->m_textWidget, QColor("#454346"), Qt::white);
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
    this->m_layout->setSpacing(5);
    this->m_nameLabel->setStyleSheet("QLabel { color: black; } ");

    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

const QFileInfo &SmartDirItemWidget::fileInfo() const {
    return m_fileInfo;
}

void SmartDirItemWidget::setFileInfo(const QFileInfo &fileInfo) {
    this->setToolTip(fileInfo.absoluteFilePath());
    this->m_fileInfo = fileInfo;

    // use simple icon first. After thumbnails generated, the icons will be replaced
    this->m_iconLabel->setPixmap(SmartDirUtils::getSimpleIcon(fileInfo, ICON_SIZE, ICON_SIZE));

    QString nameStr = fileInfo.fileName();
    this->m_nameLabel->setText(nameStr);


    QString pathStr = fileInfo.absoluteDir().absolutePath();
    QFontMetrics font = QFontMetrics(this->m_filePathLabel->font());
    int font_size = font.width(pathStr);
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

void SmartDirItemWidget::refreshIcon() {
    this->m_iconLabel->setPixmap(SmartDirUtils::getFileIcon(this->fileInfo(), ICON_SIZE, ICON_SIZE));
}

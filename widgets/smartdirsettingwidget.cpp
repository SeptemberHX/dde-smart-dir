#include "smartdirsettingwidget.h"
#include "ui_smartdirsettingwidget.h"
#include "SmartDirSettings.h"
#include <QFileDialog>
#include <QSpinBox>

SmartDirSettingWidget::SmartDirSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmartDirSettingWidget)
{
    ui->setupUi(this);
    ui->removeToolButton->setEnabled(false);

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, [this](int r) {
        ui->removeToolButton->setEnabled(r >= 0);
    });
    connect(ui->removeToolButton, &QToolButton::clicked, this, &SmartDirSettingWidget::removeCurrentPath);
    connect(ui->addToolButton, &QToolButton::clicked, this, &SmartDirSettingWidget::addNewPath);
    connect(ui->maxSizeSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int n) {
        SmartDirSettings::instance()->setItemSize(n);
    });
    connect(ui->countPerPageSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int n) {
        SmartDirSettings::instance()->setCountPerPage(n);
    });

    this->loadData();
}

SmartDirSettingWidget::~SmartDirSettingWidget()
{
    delete ui;
}

void SmartDirSettingWidget::loadData() {
    ui->listWidget->clear();
    for (const QString& dirPath : SmartDirSettings::instance()->watchedDirPaths()) {
        ui->listWidget->addItem(dirPath);
    }
    ui->maxSizeSpinBox->setValue(SmartDirSettings::instance()->getItemSize());
    ui->countPerPageSpinBox->setValue(SmartDirSettings::instance()->getCountPerPage());
}

void SmartDirSettingWidget::removeCurrentPath() {
    QString selectedPath = ui->listWidget->currentItem()->text();
    if (!selectedPath.isNull()) {
        ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
    QStringList stringList = SmartDirSettings::instance()->watchedDirPaths();
    stringList.removeOne(selectedPath);
    SmartDirSettings::instance()->setWatchedDirPaths(stringList);
}

void SmartDirSettingWidget::addNewPath() {
    QString dirPath = QFileDialog::getExistingDirectory(this);
    if (!dirPath.isNull()) {
        if (!SmartDirSettings::instance()->watchedDirPaths().contains(dirPath)) {
            ui->listWidget->addItem(dirPath);

            QStringList stringList = SmartDirSettings::instance()->watchedDirPaths();
            stringList.append(dirPath);
            SmartDirSettings::instance()->setWatchedDirPaths(stringList);
        }
    }
}

void SmartDirSettingWidget::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);
    emit closed();
}

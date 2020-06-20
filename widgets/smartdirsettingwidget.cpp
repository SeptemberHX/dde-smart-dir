#include "smartdirsettingwidget.h"
#include "ui_smartdirsettingwidget.h"
#include "SmartDirSettings.h"
#include <QFileDialog>
#include <QSpinBox>
#include <QDebug>

SmartDirSettingWidget::SmartDirSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmartDirSettingWidget)
{
    ui->setupUi(this);
    ui->removeToolButton->setEnabled(false);

    connect(ui->tableWidget, &QTableWidget::currentItemChanged, this, [this](QTableWidgetItem *item) {
        if (item == nullptr) return;

        ui->removeToolButton->setEnabled(item->row() >= 0);
    });
    connect(ui->removeToolButton, &QToolButton::clicked, this, &SmartDirSettingWidget::removeCurrentPath);
    connect(ui->addToolButton, &QToolButton::clicked, this, &SmartDirSettingWidget::addNewPath);
    connect(ui->maxSizeSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int n) {
        SmartDirSettings::instance()->setItemSize(n);
    });
    connect(ui->countPerPageSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int n) {
        SmartDirSettings::instance()->setCountPerPage(n);
    });
    connect(ui->folderCountSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int n) {
        SmartDirSettings::instance()->setDirCountPerRow(n);
    });
    connect(ui->enableFolderListCheckBox, &QCheckBox::stateChanged, this, [this]() {
        SmartDirSettings::instance()->setEnableDirList(ui->enableFolderListCheckBox->isChecked());
    });

    ui->appNameLabel->setText("DDE Smart Dir Plugin");
    ui->appVersionLabel->setText("0.4.0-rc1");

    this->loadData();
}

SmartDirSettingWidget::~SmartDirSettingWidget()
{
    delete ui;
}

void SmartDirSettingWidget::loadData() {
    ui->tableWidget->clear();
    ui->tableWidget->reset();
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(0);
    QStringList headerLabels;
    headerLabels << "Dir" << "Sub Dir";
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->setColumnWidth(0, 250);

    int r = 0;
    for (const QString& dirPath : SmartDirSettings::instance()->watchedDirPaths()) {
        ui->tableWidget->insertRow(r);
        ui->tableWidget->setItem(r, 0, new QTableWidgetItem(dirPath));
        auto *checkBox = new QCheckBox(ui->tableWidget);
        checkBox->setProperty("row", r);
        qDebug() << SmartDirSettings::instance()->getEnableDirFlagMap();
        checkBox->setChecked(SmartDirSettings::instance()->getEnableDirFlagMap().contains(dirPath) ? SmartDirSettings::instance()->getEnableDirFlagMap()[dirPath] : false);
        connect(checkBox, &QCheckBox::stateChanged, this, &SmartDirSettingWidget::subDirChanged);
        ui->tableWidget->setCellWidget(r, 1, checkBox);
        ++r;
    }
    ui->maxSizeSpinBox->setValue(SmartDirSettings::instance()->getItemSize());
    ui->countPerPageSpinBox->setValue(SmartDirSettings::instance()->getCountPerPage());
    ui->folderCountSpinBox->setValue(SmartDirSettings::instance()->getDirCountPerRow());
    ui->enableFolderListCheckBox->setChecked(SmartDirSettings::instance()->isEnableDirList());
}

void SmartDirSettingWidget::removeCurrentPath() {
    QString selectedPath = ui->tableWidget->itemAt(ui->tableWidget->currentRow(), 0)->text();
    if (!selectedPath.isNull()) {
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }
    QStringList stringList = SmartDirSettings::instance()->watchedDirPaths();
    stringList.removeOne(selectedPath);
    SmartDirSettings::instance()->setWatchedDirPaths(stringList);

    QMap<QString, bool> subFlagMap = SmartDirSettings::instance()->getEnableDirFlagMap();
    subFlagMap.remove(selectedPath);
    SmartDirSettings::instance()->setEnableDirFlagMap(subFlagMap);
}

void SmartDirSettingWidget::addNewPath() {
    QString dirPath = QFileDialog::getExistingDirectory(this);
    if (!dirPath.isNull()) {
        if (!SmartDirSettings::instance()->watchedDirPaths().contains(dirPath)) {
            int r = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(r);
            ui->tableWidget->setItem(r, 0, new QTableWidgetItem(dirPath));
            auto *checkBox = new QCheckBox(ui->tableWidget);
            checkBox->setProperty("row", r);
            connect(checkBox, &QCheckBox::stateChanged, this, &SmartDirSettingWidget::subDirChanged);
            checkBox->setChecked(false);
            ui->tableWidget->setCellWidget(r, 1, checkBox);
            QStringList stringList = SmartDirSettings::instance()->watchedDirPaths();
            stringList.append(dirPath);
            SmartDirSettings::instance()->setWatchedDirPaths(stringList);

            QMap<QString, bool> subFlagMap = SmartDirSettings::instance()->getEnableDirFlagMap();
            subFlagMap.insert(dirPath, false);
            SmartDirSettings::instance()->setEnableDirFlagMap(subFlagMap);
        }
    }
}

void SmartDirSettingWidget::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);
    emit closed();
}

void SmartDirSettingWidget::subDirChanged() {
    auto *checkBox = qobject_cast<QCheckBox*>(sender());
    QString dirPath = ui->tableWidget->item(checkBox->property("row").toInt(), 0)->text();

    QMap<QString, bool> subFlagMap = SmartDirSettings::instance()->getEnableDirFlagMap();
    subFlagMap.insert(dirPath, checkBox->isChecked());
    SmartDirSettings::instance()->setEnableDirFlagMap(subFlagMap);
}

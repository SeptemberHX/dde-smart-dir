#ifndef SMARTDIRSETTINGWIDGET_H
#define SMARTDIRSETTINGWIDGET_H

#include <QWidget>

namespace Ui {
class SmartDirSettingWidget;
}

class SmartDirSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SmartDirSettingWidget(QWidget *parent = nullptr);
    ~SmartDirSettingWidget();

    void loadData();

signals:
    void closed();

private:
    Ui::SmartDirSettingWidget *ui;

    void removeCurrentPath();
    void addNewPath();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // SMARTDIRSETTINGWIDGET_H

//
// Created by septemberhx on 2020/6/4.
//

#include "SmartDirUtils.h"
#include "SmartDirSettings.h"
#include <QDebug>
#include <QApplication>
#include "widgets/smartdirwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SmartDirWidget smartDirWidget;
    smartDirWidget.loadData(SmartDirUtils::fileInfoList(SmartDirSettings::instance()->watchedDirPaths()).mid(0, 10));
    smartDirWidget.show();

    return app.exec();
}

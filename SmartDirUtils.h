//
// Created by septemberhx on 2020/6/5.
//

#ifndef DDE_SMART_DIR_SMARTDIRUTILS_H
#define DDE_SMART_DIR_SMARTDIRUTILS_H

#include <QFileInfoList>

class SmartDirUtils {

public:
    static QFileInfoList fileInfoList(const QStringList& pathList);

};


#endif //DDE_SMART_DIR_SMARTDIRUTILS_H

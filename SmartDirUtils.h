//
// Created by septemberhx on 2020/6/5.
//

#ifndef DDE_SMART_DIR_SMARTDIRUTILS_H
#define DDE_SMART_DIR_SMARTDIRUTILS_H

#include <QFileInfoList>
#include <QCache>
#include <QPixmap>
#include <QMimeDatabase>


class SmartDirUtils {

public:
    static QFileInfoList fileInfoList(const QStringList& pathList);
    static const QPixmap& getFileIcon(const QFileInfo& fileInfo, int w, int h);
    static const QPixmap& getSimpleIcon(const QFileInfo& fileInfo, int w, int h);

private:
    static QCache<QString, QPixmap> pixmapCache;
    static QMimeDatabase db;
};


#endif //DDE_SMART_DIR_SMARTDIRUTILS_H

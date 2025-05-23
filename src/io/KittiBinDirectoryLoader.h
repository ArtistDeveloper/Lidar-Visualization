#pragma once

#include <QObject>
#include <QVector>

#include "KittiBinFileReader.h"

class KittiBinDirectoryLoader : public QObject
{
    Q_OBJECT
public:
    std::vector<std::vector<PointXYZI>> loadFromFolder(const QString &folderPath);

signals:
    void progressUpdated(int value);
};
#pragma once

#include <QObject>
#include <QVector>
#include "BinLoader.h"

class BinDataLoader : public QObject
{
    Q_OBJECT
public:
    QVector<std::vector<PointXYZI>> loadFromFolder(const QString &folderPath);

signals:
    void progressUpdated(int value);
};
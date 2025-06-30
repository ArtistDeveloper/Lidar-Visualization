#include "KittiBinDirectoryLoader.h"

#include <QDir>
#include <QCoreApplication>

std::vector<std::vector<PointXYZI>> KittiBinDirectoryLoader::loadFromFolder(const QString &folderPath)
{
    std::vector<std::vector<PointXYZI>> allPoints;
    QDir dir(folderPath);
    QStringList binFiles = dir.entryList(QStringList() << "*.bin", QDir::Files);
    for (int i = 0; i < binFiles.size(); ++i) {
        std::string path = dir.filePath(binFiles[i]).toStdString();
        auto points = KittiBinFileReader::loadKittiBinFile(path);
        allPoints.push_back(points);
        emit progressUpdated(i + 1);
        QCoreApplication::processEvents();
    }
    return allPoints;
}
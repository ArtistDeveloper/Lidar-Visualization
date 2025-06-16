#include "CustomBinFileLoader.h"
#include <QFile>
#include <QByteArray>

std::vector<std::vector<PointXYZI>> CustomBinFileLoader::loadFromFile(const QString& path)
{
    std::vector<PointXYZI> points;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return {};

    QByteArray rawData = file.readAll();
    const int pointSize = sizeof(float) * 4;
    const int count = rawData.size() / pointSize;

    const float* data = reinterpret_cast<const float*>(rawData.constData());
    for (int i = 0; i < count; ++i) {
        PointXYZI p;
        p.x = data[i * 4 + 0];
        p.y = data[i * 4 + 1];
        p.z = data[i * 4 + 2];
        p.intensity = data[i * 4 + 3];
        points.push_back(p);
    }

    return { points };
}

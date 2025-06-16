#pragma once
#include <QString>
#include <vector>
#include "PointTypes.h"

class CustomBinFileLoader {
public:
    std::vector<std::vector<PointXYZI>> loadFromFile(const QString& path);
};

#pragma once

#include <vector>
#include <fstream>

#include "PointTypes.h"

class KittiBinFileReader
{
public:
    static std::vector<PointXYZI> loadKittiBinFile(const std::string &file_path);

private:
    static constexpr float KITTI_SENSOR_HEIGHT = 1.73f;
};

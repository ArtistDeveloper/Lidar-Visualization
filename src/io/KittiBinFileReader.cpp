#include "KittiBinFileReader.h"

#include <iostream>

std::vector<PointXYZI> KittiBinFileReader::loadKittiBinFile(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    std::vector<PointXYZI> points;

    if (!file)
    {
        std::cerr << "Unable to open the file: " << file_path << std::endl;
        return points;
    }

    PointXYZI point;
    while (file.read(reinterpret_cast<char *>(&point), sizeof(PointXYZI)))
    {
        point.z = point.z + KITTI_SENSOR_HEIGHT;
        points.push_back(point);
    }

    return points;
}
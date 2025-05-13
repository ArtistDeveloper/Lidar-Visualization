#include <iostream>
#include "BinLoader.h"

std::vector<PointXYZI> BinLoader::loadKittiBinFile(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    std::vector<PointXYZI> points;

    if (!file)
    {
        std::cerr << "파일을 열 수 없습니다: " << file_path << std::endl;
        return points;
    }

    PointXYZI point;
    while (file.read(reinterpret_cast<char *>(&point), sizeof(PointXYZI)))
    {
        points.push_back(point);
    }

    std::cout << "점 개수 : " << points.size() << std::endl;
    return points;
}
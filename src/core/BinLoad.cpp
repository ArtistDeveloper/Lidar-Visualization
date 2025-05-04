#include <iostream>
#include "BinLoad.h"

std::vector<PointXYZI> loadKittiBinFile(const std::string &file_path)
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

    return points;
}

void TestLoadBinFile()
{
    // point cloud data test
    std::string path = "C:/Users/ParkJunsu/Desktop/raw_data_downloader/2011_09_28/2011_09_28_drive_0001_sync/velodyne_points/data/0000000000.bin";
    std::vector<PointXYZI> cloud = loadKittiBinFile(path);

    std::cout << "점 개수 : " << cloud.size() << std::endl;
}
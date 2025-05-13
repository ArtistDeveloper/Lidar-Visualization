#pragma once

#include <vector>
#include <fstream>

struct PointXYZI
{
    float x, y, z, intensity;
};

class BinLoader
{
public:
    static std::vector<PointXYZI> loadKittiBinFile(const std::string &file_path);
};

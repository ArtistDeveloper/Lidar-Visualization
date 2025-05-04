#include <vector>
#include <fstream>

struct PointXYZI
{
    float x, y, z, intensity;
};

std::vector<PointXYZI> loadKittiBinFile(const std::string &file_path);
void TestLoadBinFile();
#include "PointCloudRepository.h"

PointCloudRepository::PointCloudRepository(QObject* parent)
    : QObject(parent)
{}

void PointCloudRepository::setPointCloudData(std::vector<std::vector<PointXYZI>> data)
{
    *data_ = std::move(data);   // 소유권 이동
    emit dataReset();
}

const std::vector<std::vector<PointXYZI>>& PointCloudRepository::pointCloudData() const
{
    return *data_;
}
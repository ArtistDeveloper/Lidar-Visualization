#include "PointCloudRepository.h"

PointCloudRepository::PointCloudRepository(QObject* parent)
    : QObject(parent)
{}

void PointCloudRepository::setPointCloudData(std::shared_ptr<std::vector<std::vector<PointXYZI>>> data)
{
    data_ = std::move(data); // 포인터 교체 (데이터 복사 없음
    emit dataReset();
}

const std::vector<std::vector<PointXYZI>>& PointCloudRepository::pointCloudData() const
{
    return *data_;
}
#pragma once
#include <QObject>
#include <memory>
#include <vector>
#include "PointTypes.h"

class PointCloudRepository : public QObject
{
    Q_OBJECT
public:
    explicit PointCloudRepository(QObject *parent = nullptr);

    // loader 쪽에서 std::move로 넘겨 받음
    void setPointCloudData(std::shared_ptr<std::vector<std::vector<PointXYZI>>> data);

    std::shared_ptr<const std::vector<std::vector<PointXYZI>>> dataPtr() const
    {
        return data_; // 카운트만 올려서 반환
    }

signals:
    void dataReset(); // 데이터 전체 교체 알림

private:
    std::shared_ptr<std::vector<std::vector<PointXYZI>>> data_ = std::make_shared<std::vector<std::vector<PointXYZI>>>();
};

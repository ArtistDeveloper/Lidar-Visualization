#pragma once

#include <QObject>
#include <QTimer>

#include "PointTypes.h"

class PointCloudPlayer : public QObject
{
    Q_OBJECT

public:
    explicit PointCloudPlayer(QObject *parent = nullptr);
    void setEntireData(const std::vector<std::vector<PointXYZI>> &data);
    void play();
    void pause();
    void nextFrame();
    void prevFrame();
    void setFrame(int index);

    int currentFrame() const;
    int totalFrames() const;

signals:
    void frameChanged(const std::vector<PointXYZI>& frame);
    void frameIndexChanged(int index);
    void playbackStopped();

private slots:
    void onTimeout();

private:
    std::vector<std::vector<PointXYZI>> data_;
    const int KITTI_FRAME_RATE = 100;
    int currentFrame_ = 0;
    QTimer timer_;
};

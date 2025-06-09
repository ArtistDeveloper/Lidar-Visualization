#include "PointCloudPlayer.h"

#include <QDebug>

PointCloudPlayer::PointCloudPlayer(QObject *parent) : QObject(parent)
{
    // connect(&timer_, &QTimer::timeout, this, &PointCloudPlayer::onTimeout);
    connect(&timer_, &QTimer::timeout, this, &PointCloudPlayer::onTimeout);
    timer_.setInterval(500);  // 0.5초 간격
}

void PointCloudPlayer::setEntireData(const std::vector<std::vector<PointXYZI>> &data)
{
    data_ = data;
    emit frameChanged(data_[0]);
}

void PointCloudPlayer::play()
{
    if (!timer_.isActive())
        timer_.start();
}

void PointCloudPlayer::pause()
{
    timer_.stop();
}

void PointCloudPlayer::nextFrame()
{
    if (data_.empty())
        return;

    if (currentFrame_ < static_cast<int>(data_.size()) - 1)
    {
        ++currentFrame_;
        emit frameChanged(data_[currentFrame_]);
        emit frameIndexChanged(currentFrame_);
    }
}

void PointCloudPlayer::prevFrame()
{
    if (data_.empty())
        return;

    if (currentFrame_ > 0)
    {
        --currentFrame_;
        emit frameChanged(data_[currentFrame_]);
        emit frameIndexChanged(currentFrame_);
    }
}

void PointCloudPlayer::setFrame(int index)
{
    if (data_.empty() || index < 0 || index >= static_cast<int>(data_.size()))
        return;

    currentFrame_ = index;
    emit frameChanged(data_[currentFrame_]);
    emit frameIndexChanged(currentFrame_);
}

int PointCloudPlayer::currentFrame() const
{
    return currentFrame_;
}

int PointCloudPlayer::totalFrames() const
{
    throw std::logic_error("totalFrames() is not implemented yet.");
}

void PointCloudPlayer::onTimeout()
{
    if (currentFrame_ >= static_cast<int>(data_.size()) - 1) {
        timer_.stop();
        emit playbackStopped();  // 자동 중지 시 UI 알림
        return;
    }

    nextFrame();
}
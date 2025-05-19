#include "PointCloudPlayer.h"

#include <QDebug>

PointCloudPlayer::PointCloudPlayer(QObject *parent) : QObject(parent)
{
    // connect(&timer_, &QTimer::timeout, this, &PointCloudPlayer::onTimeout);
}

void PointCloudPlayer::setEntireData(const std::vector<std::vector<PointXYZI>> &data)
{
    data_ = data;
    emit frameChanged(data_[0]);
}

void PointCloudPlayer::play()
{
    throw std::logic_error("play() is not implemented yet.");
}

void PointCloudPlayer::pause()
{
    throw std::logic_error("pause() is not implemented yet.");
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
    throw std::logic_error("onTimeout() is not implemented yet.");
}

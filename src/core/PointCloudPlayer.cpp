#include "PointCloudPlayer.h"

PointCloudPlayer::PointCloudPlayer(QObject *parent) : QObject(parent) {
    // connect(&timer_, &QTimer::timeout, this, &PointCloudPlayer::onTimeout);
}

void PointCloudPlayer::play() {
    throw std::logic_error("play() is not implemented yet.");
}

void PointCloudPlayer::pause() {
    throw std::logic_error("pause() is not implemented yet.");
}

void PointCloudPlayer::nextFrame() {
    throw std::logic_error("nextFrame() is not implemented yet.");
}

void PointCloudPlayer::prevFrame() {
    throw std::logic_error("prevFrame() is not implemented yet.");
}

void PointCloudPlayer::setFrame(int index) {
    throw std::logic_error("setFrame() is not implemented yet.");
}

int PointCloudPlayer::currentFrame() const {
    throw std::logic_error("currentFrame() is not implemented yet.");
}

int PointCloudPlayer::totalFrames() const {
    throw std::logic_error("totalFrames() is not implemented yet.");
}

void PointCloudPlayer::onTimeout() {
    throw std::logic_error("onTimeout() is not implemented yet.");
}

#include "OrbitCamera.h"
#include <QtMath>

OrbitCamera::OrbitCamera() { rebuildView(); }

void OrbitCamera::rotate(float dYawDeg, float dPitchDeg)
{
    // yaw_ = qBound(-89.f, yaw_ - dYawDeg * speedRot_, 89.f);
    yaw_ -= dYawDeg * speedRot_;
    pitch_ = qBound(-89.f, pitch_ - dPitchDeg * speedRot_, 89.f);
    rebuildView();
}

void OrbitCamera::zoom(float wheelDelta)
{
    radius_ *= std::pow(speedZoom_, -wheelDelta); // 휠 ↑: 확대
    radius_ = qMax(0.1f, radius_);
    rebuildView();
}

void OrbitCamera::rebuildView()
{
    const float yawRad = qDegreesToRadians(yaw_);
    const float pitchRad = qDegreesToRadians(pitch_);

    QVector3D eye{
        radius_ * std::cos(pitchRad) * std::sin(yawRad),
        radius_ * std::sin(pitchRad),
        radius_ * std::cos(pitchRad) * std::cos(yawRad)};
    QVector3D up{0, 1, 0};

    view_.setToIdentity();
    view_.lookAt(eye + center_, center_, up);
}

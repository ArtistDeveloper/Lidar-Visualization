#include "OrbitCamera.h"
#include <QtMath>

OrbitCamera::OrbitCamera() { rebuildView(); }

void OrbitCamera::rotate(float dYawDeg, float dPitchDeg)
{
    // qDebug() << "dYawDeg, pithDeg: " << dYawDeg << " " << dPitchDeg;
    yaw_ = std::fmod(yaw_ - dYawDeg * speedRot_, 360.f);
    // qDebug() << "yaw: " << yaw_;
    if (yaw_ < 0.f)
        yaw_ += 360.f;

    pitch_ = qBound(-89.f, pitch_ - dPitchDeg * speedRot_, 89.f);
    rebuildView();
}

void OrbitCamera::zoom(int wheelDelta)
{
    constexpr float kQtStep = 120.0f;
    float steps = wheelDelta / kQtStep;

    float factor = std::pow(zoomStep_, -steps);

    radius_ = qBound(minRadius_, radius_ * factor, maxRadius_);
    rebuildView();
}

void OrbitCamera::pan(float dxPx, float dyPx, float viewHpx, float fovDeg)
{
    const float k = 2.f * radius_ * std::tan(qDegreesToRadians(fovDeg * 0.5f)) / viewHpx;

    // 현재 카메라 축 구하기
    const float yawRad = qDegreesToRadians(yaw_);
    const float pitchRad = qDegreesToRadians(pitch_);

    // camera 방향
    // 구면 좌표를 직교 좌표로 변환
    QVector3D forward{
        -std::cos(pitchRad) * std::sin(yawRad),
        -std::sin(pitchRad),
        -std::cos(pitchRad) * std::cos(yawRad)};
    forward.normalize();

    QVector3D worldUp{0, 1, 0};
    QVector3D right = QVector3D::crossProduct(forward, worldUp).normalized();
    QVector3D up = QVector3D::crossProduct(right, forward); // 이미 정규화됨

    center_ -= right * dxPx * k;
    center_ += up * dyPx * k;

    rebuildView();
}

void OrbitCamera::rebuildView()
{
    const float yawRad = qDegreesToRadians(yaw_);
    const float pitchRad = qDegreesToRadians(pitch_);

    // radius만큼 떨어진 카메라의 위치 벡터
    QVector3D camera{
        radius_ * std::cos(pitchRad) * std::sin(yawRad),
        radius_ * std::sin(pitchRad),
        radius_ * std::cos(pitchRad) * std::cos(yawRad)
    };
    QVector3D up{0, 1, 0};

    // eye + center가 카메라의 최종 위치
    view_.setToIdentity();
    view_.lookAt(camera + center_, center_, up);
}

#pragma once
#include <QMatrix4x4>
#include <QVector3D>

class OrbitCamera
{
public:
    OrbitCamera();

    void rotate(float dYawDeg, float dPitchDeg);   // 마우스 드래그
    void zoom(float wheelDelta);                   // 휠
    const QMatrix4x4& getViewMatrix() const { return view_; }

private:
    void rebuildView();

    /* 상태 */
    float yaw_   {  0.f };   // deg
    float pitch_ {  0.f };   // deg
    float radius_{ 3.f };    // distance
    const float speedRot_ = 0.3f;   // 마우스 감도
    const float speedZoom_ = 1.001f;

    QVector3D center_{0,0,0};
    QMatrix4x4 view_;
};

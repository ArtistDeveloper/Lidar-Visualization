#pragma once
#include <QMatrix4x4>
#include <QVector3D>

class OrbitCamera
{
public:
    OrbitCamera();

    void rotate(float dYawDeg, float dPitchDeg);
    void zoom(float wheelDelta);
    void pan(float dxPixels, float dyPixels);

    const QMatrix4x4& getViewMatrix() const { return view_; }
    

private:
    void rebuildView();

    float yaw_   {  0.f };   // deg
    float pitch_ {  0.f };   // deg
    float radius_{ 3.f };    // distance
    const float speedRot_ = 0.3f;   // 마우스 감도
    const float speedZoom_ = 1.001f;
    const float speedPan_  = 0.001f;

    QVector3D center_{0,0,0};
    QMatrix4x4 view_;
};

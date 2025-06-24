#pragma once
#include <QMatrix4x4>
#include <QVector3D>

class OrbitCamera
{
public:
    OrbitCamera();

    void rotate(float dYawDeg, float dPitchDeg);
    void zoom(int wheelDelta);
    void pan(float dxPx, float dyPx, float viewportHpx, float fovDeg);

    const QMatrix4x4 &getViewMatrix() const { return view_; }
    float getRadius() const { return radius_; }

private:
    void rebuildView();

    float yaw_{0.f};              // deg
    float pitch_{0.f};            // deg
    float radius_{3.f};           // distance
    const float speedRot_ = 0.3f; // 마우스 감도
    const float speedZoom_ = 1.001f;
    const float speedPan_ = 0.001f;
    
    // Zoom 최소 및 최대값 지정
    const float minRadius_ = 0.2f;
    const float maxRadius_ = 500.f;
    const float zoomStep_ = 1.15f;

    QVector3D center_{0, 0, 0};
    QMatrix4x4 view_;
};

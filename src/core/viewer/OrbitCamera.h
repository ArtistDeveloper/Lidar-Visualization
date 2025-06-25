#pragma once
#include <QMatrix4x4>
#include <QVector3D>

class OrbitCamera
{
public:
    OrbitCamera();

    static constexpr float defaultYaw = 0.f; 
    static constexpr float defaultPitch = 30.f;
    static constexpr float defaultRadius = 3.f;

    void rotate(float dYawDeg, float dPitchDeg);
    void zoom(int wheelDelta);
    void pan(float dxPx, float dyPx, float viewportHpx, float fovDeg);
    void reset();

    const QMatrix4x4 &getViewMatrix() const { return view_; }
    float getRadius() const { return radius_; }

private:
    void rebuildView();

    float yaw_{defaultYaw}; // 0~360도 범위로 스케일링해서 사용. center_주위로 얼마나 회전한지 각도 저장
    float pitch_{defaultPitch};
    float radius_{defaultRadius};
    const float speedRot_ = 0.3f;
    const float speedPan_ = 0.001f;

    // Zoom 최소 및 최대값 지정
    const float minRadius_ = 0.2f;
    const float maxRadius_ = 500.f;
    const float zoomStep_ = 1.15f;

    QVector3D center_{0, 0, 0};
    QMatrix4x4 view_;
};

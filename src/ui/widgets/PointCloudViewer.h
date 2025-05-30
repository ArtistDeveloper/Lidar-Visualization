#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

#include "PointTypes.h"

class PointCloudViewer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    PointCloudViewer(QWidget *parent = nullptr);
    ~PointCloudViewer();

public slots:
    void setPointCloudData(const std::vector<PointXYZI> &points);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void updateViewMatrix();

    std::unique_ptr<QOpenGLShaderProgram> m_program = nullptr;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    std::vector<PointXYZI> m_pointCloud;

    // Arcball Camera
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projMatrix;
    QVector3D m_eye = {0.0f, 0.0f, 3.0f};
    QVector3D m_center = {0.0f, 0.0f, 0.0f};
    QVector3D m_up = {0.0f, 1.0f, 0.0f};
    QPoint m_lastMousePos;
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    float m_radius = 3.0f;
    float m_rotationSpeed = 0.5f;
    float m_zoom = 1.0f;
};

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

#include "PointTypes.h"

class OrbitCamera;

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
    QMatrix4x4 m_projMatrix;
    QPoint m_lastMousePos;

    std::unique_ptr<OrbitCamera> camera_; 
};

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QElapsedTimer>

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
    void keyPressEvent(QKeyEvent *e) override;

private:
    void updateViewMatrix();
    void drawAxisLabels(QPainter &p, const QMatrix4x4 &mvp, int viewW, int viewH);

    std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    QMatrix4x4 projMatrix_;
    QPoint lastMousePos_;

    std::vector<PointXYZI> pointCloud_;
    std::unique_ptr<OrbitCamera> camera_;

    /* --- Grid --- */
    void initGrid();
    GLuint gridVao_{0};
    GLuint gridVbo_{0};
    int gridVertexCount_{0};
    std::unique_ptr<QOpenGLShaderProgram> gridProgram_;
    bool drawGrid_{true}; // 토글용 플래그 (나중에 UI에서 변경하면 됨)

    void buildGrid(float step);
    void updateGridIfNeeded();    // 스텝 변경 감지
    float currentGridStep_{-1.f}; // 현재 생성된 간격

    /* --- FPS 계측 --- */
    bool           showFps_{true};   // ‘]’ 로 토글
    QElapsedTimer  fpsTimer_;
    int            fpsFrameCount_{0};
    float          currentFps_{0.f};
};

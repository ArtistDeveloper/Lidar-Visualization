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
    void setPointCloudData(const std::vector<PointXYZI>& points);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    std::unique_ptr<QOpenGLShaderProgram> m_program = nullptr;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    std::vector<PointXYZI> m_pointCloud;
};

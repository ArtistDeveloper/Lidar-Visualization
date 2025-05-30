#include "PointCloudViewer.h"

#include <QDebug>
#include <QMouseEvent>

#include "PointTypes.h"
#include "ShaderProgram.h"

PointCloudViewer::PointCloudViewer(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

PointCloudViewer::~PointCloudViewer()
{
    makeCurrent();
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    m_program.reset();
    doneCurrent();
}

void PointCloudViewer::initializeGL()
{
    m_viewMatrix.lookAt(m_eye, m_center, m_up);
    // setFocusPolicy(Qt::StrongFocus); // 키보드 및 마우스 포커스를 받도록 설정
    // setMouseTracking(true);          // 마우스 무브 추적 (마우스 클릭 없이도 움직임 감지 가능)
    initializeOpenGLFunctions();

    // 셰이더 프로그램 생성
    m_program = ShaderProgram::create(":/shader/pointcloud.vs", ":/shader/pointcloud.fs");

    // OpenGL Buffer
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // 초기에는 빈 버퍼를 할당
    glBufferData(GL_ARRAY_BUFFER, 1, nullptr, GL_DYNAMIC_DRAW);

    // Vertex Attribute: position (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointXYZI), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // NOTE: Point Cloud의 depth에 따른 렌더링 순서가 이상한 것 같으면 추후 활성화
    glEnable(GL_DEPTH_TEST);
}

void PointCloudViewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_projMatrix.setToIdentity();
    m_projMatrix.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
    // m_projMatrix.perspective(45.0f, float(w) / float(h), 0.01f, 500.0f);
}

void PointCloudViewer::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    // MVP 행렬 계산 및 전송
    QMatrix4x4 mvp = m_projMatrix * m_viewMatrix;
    m_program->setUniformValue("u_mvp", mvp);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_pointCloud.size());
    glBindVertexArray(0);

    m_program->release();
}

void PointCloudViewer::setPointCloudData(const std::vector<PointXYZI> &points)
{
    makeCurrent();

    m_pointCloud = points;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_pointCloud.size() * sizeof(PointXYZI), m_pointCloud.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    update();
}

void PointCloudViewer::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePos = event->pos();
}

void PointCloudViewer::updateViewMatrix()
{
    float yawRad = qDegreesToRadians(m_yaw);
    float pitchRad = qDegreesToRadians(m_pitch);

    QVector3D offset;
    offset.setX(m_radius * std::cos(pitchRad) * std::sin(yawRad));
    offset.setY(m_radius * std::sin(pitchRad));
    offset.setZ(m_radius * std::cos(pitchRad) * std::cos(yawRad));

    m_eye = m_center + offset;
    m_up = {0, 1, 0};

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_eye, m_center, m_up);
    update();
}

void PointCloudViewer::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastMousePos.x();
    int dy = event->y() - m_lastMousePos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        m_yaw -= dx * m_rotationSpeed;
        m_pitch -= dy * m_rotationSpeed;

        m_yaw = std::clamp(m_yaw, -89.0f, 89.0f);
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
        
        updateViewMatrix();
    }

    m_lastMousePos = event->pos();
}

void PointCloudViewer::wheelEvent(QWheelEvent *event)
{
    m_zoom *= std::pow(1.001f, event->angleDelta().y());
    m_radius = 3.0f / m_zoom;
    updateViewMatrix();
}

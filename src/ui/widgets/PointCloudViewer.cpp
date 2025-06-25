#include "PointCloudViewer.h"

#include <QDebug>
#include <QMouseEvent>

#include "PointTypes.h"
#include "ShaderProgram.h"
#include "OrbitCamera.h"

PointCloudViewer::PointCloudViewer(QWidget *parent)
    : QOpenGLWidget(parent),
      camera_(std::make_unique<OrbitCamera>())
{
}

PointCloudViewer::~PointCloudViewer()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    program_.reset();
    doneCurrent();
}

void PointCloudViewer::initializeGL()
{
    // setFocusPolicy(Qt::StrongFocus); // 키보드 및 마우스 포커스를 받도록 설정
    // setMouseTracking(true);          // 마우스 무브 추적 (마우스 클릭 없이도 움직임 감지 가능)
    initializeOpenGLFunctions();

    // 셰이더 프로그램 생성
    program_ = ShaderProgram::create(":/shader/pointcloud.vs", ":/shader/pointcloud.fs");

    // OpenGL Buffer
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // 초기에는 빈 버퍼를 할당
    glBufferData(GL_ARRAY_BUFFER, 1, nullptr, GL_DYNAMIC_DRAW);

    // Vertex Attribute: position (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointXYZI), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    // initGrid(); // Grid VAO/VBO 준비
    buildGrid(1.0f);
}

void PointCloudViewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    projMatrix_.setToIdentity();
    projMatrix_.perspective(45.0f, float(w) / float(h), 0.1f, 600.0f);
}

void PointCloudViewer::paintGL()
{
    updateGridIfNeeded();

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program_->bind();

    // MVP 행렬 계산 및 전송
    QMatrix4x4 mvp = projMatrix_ * camera_->getViewMatrix();
    program_->setUniformValue("u_mvp", mvp);

    glBindVertexArray(vao_);
    glDrawArrays(GL_POINTS, 0, pointCloud_.size());
    glBindVertexArray(0);

    program_->release();

    /* --- Grid --- */
    if (drawGrid_)
    {
        gridProgram_->bind();
        QMatrix4x4 mvp = projMatrix_ * camera_->getViewMatrix();
        gridProgram_->setUniformValue("u_mvp", mvp);
        glBindVertexArray(gridVao_);
        glDrawArrays(GL_LINES, 0, gridVertexCount_);
        glBindVertexArray(0);
        gridProgram_->release();
    }
}

void PointCloudViewer::setPointCloudData(const std::vector<PointXYZI> &src)
{
    makeCurrent();

    pointCloud_.resize(src.size());
    std::transform(src.begin(), src.end(), pointCloud_.begin(),
                   [](const PointXYZI &p)
                   {
                       // (x, y, z) -> (x, z, -y)
                       return PointXYZI{p.x, p.z, -p.y, p.intensity};
                   });

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 pointCloud_.size() * sizeof(PointXYZI),
                 pointCloud_.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    update();
}

void PointCloudViewer::mousePressEvent(QMouseEvent *e)
{
    lastMousePos_ = e->pos();
}

void PointCloudViewer::mouseMoveEvent(QMouseEvent *e)
{
    const QPoint delta = e->pos() - lastMousePos_;

    if (e->buttons() & Qt::RightButton)
    {
        camera_->rotate(delta.x(), delta.y());
    }
    else if (e->buttons() & Qt::LeftButton)
    {
        camera_->pan(delta.x(), -delta.y(),
                     height(),
                     45.0f);
    }

    update();
    lastMousePos_ = e->pos();
}

void PointCloudViewer::wheelEvent(QWheelEvent *e)
{
    camera_->zoom(e->angleDelta().y());
    update();
}

void PointCloudViewer::buildGrid(float step)
{
    // 이전 버퍼 존재하면 정리
    if (gridVbo_)  glDeleteBuffers(1, &gridVbo_);
    if (gridVao_)  glDeleteVertexArrays(1, &gridVao_);
    gridVao_ = gridVbo_ = 0;

    // 셰이더는 한 번만 만들면 되므로 처음 호출 때 초기화
    if (!gridProgram_)
        gridProgram_ = ShaderProgram::create(":/shader/grid.vs", ":/shader/grid.fs");

    constexpr float EXTENT = 200.f; // -200 m ~ +200 m
    std::vector<float> v;
    const int lines = static_cast<int>((EXTENT * 2) / step) + 1;
    v.reserve(lines * 4 * 3);

    // X축과 평행(Z 고정)
    for (float z = -EXTENT; z <= EXTENT + 0.001f; z += step)
        v.insert(v.end(), {-EXTENT, 0.f, z,
                             EXTENT, 0.f, z});
    // Z축과 평행(X 고정)
    for (float x = -EXTENT; x <= EXTENT + 0.001f; x += step)
        v.insert(v.end(), {x, 0.f, -EXTENT,
                           x, 0.f,  EXTENT});

    gridVertexCount_ = static_cast<int>(v.size() / 3);

    glGenVertexArrays(1, &gridVao_);
    glGenBuffers(1, &gridVbo_);

    glBindVertexArray(gridVao_);
    glBindBuffer(GL_ARRAY_BUFFER, gridVbo_);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(float), v.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PointCloudViewer::updateGridIfNeeded()
{
    const float r = camera_->getRadius();

    float desired;
    if      (r > 150.f) desired = 10.f;
    else if (r >  70.f) desired =  5.f;
    else if (r >  25.f) desired =  2.f;
    else if (r >   7.f) desired =  1.f;
    else                desired =  0.5f;

    if (qFuzzyCompare(desired, currentGridStep_)) // 이미 그 간격이면 패스
        return;

    currentGridStep_ = desired;
    buildGrid(desired); // 새 VBO 생성
}
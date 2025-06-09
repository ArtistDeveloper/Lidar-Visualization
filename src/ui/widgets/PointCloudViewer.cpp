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
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    m_program.reset();
    doneCurrent();
}

void PointCloudViewer::initializeGL()
{
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

    glEnable(GL_DEPTH_TEST);
    initGrid(); // Grid VAO/VBO 준비
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
    QMatrix4x4 mvp = m_projMatrix * camera_->getViewMatrix();
    m_program->setUniformValue("u_mvp", mvp);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_pointCloud.size());
    glBindVertexArray(0);

    m_program->release();

    /* --- Grid --- */
    if (drawGrid_)
    {
        gridProgram_->bind();
        QMatrix4x4 mvp = m_projMatrix * camera_->getViewMatrix();
        gridProgram_->setUniformValue("u_mvp", mvp);
        glBindVertexArray(gridVao_);
        glDrawArrays(GL_LINES, 0, gridVertexCount_);
        glBindVertexArray(0);
        gridProgram_->release();
    }
}

void PointCloudViewer::setPointCloudData(const std::vector<PointXYZI>& src)
{
    makeCurrent();

    m_pointCloud.resize(src.size());
    std::transform(src.begin(), src.end(), m_pointCloud.begin(),
                   [](const PointXYZI& p){
                       // (x, y, z) -> (x, z, -y)
                       return PointXYZI{ p.x, p.z, -p.y, p.intensity };
                   });

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_pointCloud.size() * sizeof(PointXYZI),
                 m_pointCloud.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    update();
}

void PointCloudViewer::mousePressEvent(QMouseEvent *e)
{
    m_lastMousePos = e->pos();
}

void PointCloudViewer::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        const QPoint delta = e->pos() - m_lastMousePos;
        camera_->rotate(delta.x(), delta.y());
        update(); // 다시 그리기
    }
    m_lastMousePos = e->pos();
}

void PointCloudViewer::wheelEvent(QWheelEvent *e)
{
    camera_->zoom(e->angleDelta().y());
    update();
}

// Grid용 POC 메서드
void PointCloudViewer::initGrid()
{
    // 1. 셰이더
    gridProgram_ = ShaderProgram::create(":/shader/grid.vs", ":/shader/grid.fs");

    // 2. 정점 데이터 생성 (XZ 평면, Y=0)
    constexpr int GRID_SIZE = 50;     // -50 ~ +50
    constexpr float STEP = 1.0f;
    std::vector<float> verts;
    verts.reserve((GRID_SIZE*2+1)*4*3);   // 404*3 floats

    // x 축 평행선들 (Z 고정)
    for (int i=-GRID_SIZE; i<=GRID_SIZE; ++i) {
        float z = i * STEP;
        verts.insert(verts.end(), { -GRID_SIZE*STEP, 0.f, z,
                                     GRID_SIZE*STEP, 0.f, z });
    }
    // z 축 평행선들 (X 고정)
    for (int i=-GRID_SIZE; i<=GRID_SIZE; ++i) {
        float x = i * STEP;
        verts.insert(verts.end(), { x, 0.f, -GRID_SIZE*STEP,
                                    x, 0.f,  GRID_SIZE*STEP });
    }
    gridVertexCount_ = static_cast<int>(verts.size() / 3);

    // 3. OpenGL VAO/VBO
    glGenVertexArrays(1, &gridVao_);
    glGenBuffers(1, &gridVbo_);

    glBindVertexArray(gridVao_);
    glBindBuffer(GL_ARRAY_BUFFER, gridVbo_);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(float),
                 verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

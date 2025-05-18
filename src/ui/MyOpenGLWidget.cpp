#include <QDebug>

#include "MyOpenGLWidget.h"
#include "BinLoader.h"
#include "ShaderProgram.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    makeCurrent();
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    delete m_program;
    doneCurrent();
}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Load point cloud
    std::string path = "C:/Users/ParkJunsu/Desktop/raw_data_downloader/2011_09_28/2011_09_28_drive_0001_sync/velodyne_points/data/0000000000.bin";
    m_pointCloud = BinLoader::loadKittiBinFile(path);

    // Shader
    static const char *vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 position;
    void main() {
            gl_PointSize = 2.0;
            gl_Position = vec4(position * 0.02, 1.0); // point cloud를 시각화 하기 위해 scaling 테스트
        }
    )";

    static const char *fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 1.0, 1.0, 1.0); // white
        }
    )";

    auto test = ShaderProgram::create("./src/shader/pointcloud.vs", "./src/shader/pointcloud.fs");

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
    m_program->link();

    // OpenGL Buffer
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_pointCloud.size() * sizeof(PointXYZI), m_pointCloud.data(), GL_STATIC_DRAW);

    // Vertex Attribute: position (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointXYZI), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // glEnable(GL_DEPTH_TEST);
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MyOpenGLWidget::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    glBindVertexArray(m_vao);

    glDrawArrays(GL_POINTS, 0, m_pointCloud.size()); 

    glBindVertexArray(0);
    m_program->release();
}
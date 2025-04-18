#include "MyOpenGLWidget.h"
#include <QDebug>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {
}

MyOpenGLWidget::~MyOpenGLWidget() {
    makeCurrent();
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    delete m_program;
    doneCurrent();
}

void MyOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();

    static const char *vertexShaderSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 position;
        void main() {
            gl_Position = vec4(position, 1.0);
        }
    )";

    static const char *fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
    m_program->link();

    GLfloat vertices[] = {
         0.0f,  0.5f, 0.0f,  // 위
        -0.5f, -0.5f, 0.0f,  // 좌하
         0.5f, -0.5f, 0.0f   // 우하
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MyOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void MyOpenGLWidget::paintGL() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    m_program->release();
}

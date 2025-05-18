#include "ShaderProgram.h"

#include <QDir>
#include <QResource>

std::unique_ptr<QOpenGLShaderProgram> ShaderProgram::create(const QString &vertPath, const QString &fragPath)
{
    auto program = std::make_unique<QOpenGLShaderProgram>();

    QFile file(":/shader/pointcloud.vs");
    if (file.exists())
    {
        qDebug() << "Shader file found in QRC!";
    }
    else
    {
        qWarning() << "Shader file NOT found in QRC!";
    }

    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertPath) ||
        !program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragPath) ||
        !program->link())
    {
        qDebug() << QDir::currentPath();
        qDebug() << QResource(":/shaders/pointcloud.vs").isValid();
        qWarning() << "Shader compilation/link failed:" << program->log();
        return nullptr;
    }

    return program;
}

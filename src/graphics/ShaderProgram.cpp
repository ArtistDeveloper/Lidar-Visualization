#include "ShaderProgram.h"

#include <QDir>
#include <QResource>

std::unique_ptr<QOpenGLShaderProgram> ShaderProgram::create(const QString &vertPath, const QString &fragPath)
{
    auto program = std::make_unique<QOpenGLShaderProgram>();

    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertPath) ||
        !program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragPath) ||
        !program->link())
    {
        qDebug() << QDir::currentPath();
        qDebug() << QResource(vertPath).isValid();
        qDebug() << QResource(fragPath).isValid();
        qWarning() << "Shader compilation/link failed:" << program->log();
        return nullptr;
    }

    return std::move(program);
}

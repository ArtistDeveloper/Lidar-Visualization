#pragma once

#include <QOpenGLShaderProgram>
#include <QString>

class ShaderProgram
{
    public:
        static std::unique_ptr<QOpenGLShaderProgram> create(const QString& vertPath, const QString& fragPath);
};
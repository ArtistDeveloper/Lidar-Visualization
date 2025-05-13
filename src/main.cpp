#include <QApplication>
#include <QWidget>

#include "MyOpenGLWidget.h"
#include "BinLoad.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);
    MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}

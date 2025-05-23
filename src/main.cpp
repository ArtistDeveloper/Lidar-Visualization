#include <QApplication>
#include <QWidget>

// #include "PointCloudViewer.h"
#include "PointTypes.h"
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

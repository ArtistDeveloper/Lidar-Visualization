#include <QApplication>
#include <QWidget>
#include <QSurfaceFormat>

#include "PointTypes.h"
#include "MainWindow.h"
#include "PointCloudRepository.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);
    
    auto repo = std::make_shared<PointCloudRepository>(&app); // App이 삭제될 때 같이 메모리 해제. QObject에 종속하면 부모 오브젝트가 정리될때, 자식 오브젝트도 정리 가능
    MainWindow w(repo);
    w.show();

    return app.exec();
}

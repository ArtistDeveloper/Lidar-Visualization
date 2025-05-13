#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "MyOpenGLWidget.h"
#include "BinLoad.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    // 메인 윈도우
    QMainWindow window;

    // 메인 중앙 위젯 생성 (레이아웃을 넣을 컨테이너)
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    // OpenGL 위젯
    MyOpenGLWidget *glWidget = new MyOpenGLWidget;

    // 버튼 생성
    QPushButton *button = new QPushButton("눌러봐요");

    // 버튼 눌렀을 때 출력
    QObject::connect(button, &QPushButton::clicked, []()
                     { qDebug() << "버튼이 눌렸습니다!"; });

    // 레이아웃에 위젯들 추가
    layout->addWidget(glWidget, 1); // OpenGL 위젯 (비중 1)
    layout->addWidget(button);      // 버튼 (고정 높이)

    centralWidget->setLayout(layout);
    window.setCentralWidget(centralWidget);

    window.resize(800, 600);
    window.show();

    return app.exec();
}

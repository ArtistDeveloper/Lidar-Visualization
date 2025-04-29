#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "MyOpenGLWidget.h"


#include <iostream>
#include <fstream>
#include <vector>

struct PointXYZI {
    float x, y, z, intensity;
};

std::vector<PointXYZI> loadKittiBinFile(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    std::vector<PointXYZI> points;

    if (!file) {
        std::cerr << "파일을 열 수 없습니다: " << file_path << std::endl;
        return points;
    }

    PointXYZI point;
    while (file.read(reinterpret_cast<char*>(&point), sizeof(PointXYZI))) {
        points.push_back(point);
    }

    return points;
}



int main(int argc, char *argv[]) {
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
    QObject::connect(button, &QPushButton::clicked, [](){
        qDebug() << "버튼이 눌렸습니다!";
    });

    // 레이아웃에 위젯들 추가
    layout->addWidget(glWidget, 1);  // OpenGL 위젯 (비중 1)
    layout->addWidget(button);       // 버튼 (고정 높이)

    centralWidget->setLayout(layout);
    window.setCentralWidget(centralWidget);

    window.resize(800, 600);
    window.show();

    // point cloud data test
    std::string path = "C:/Users/ParkJunsu/Desktop/raw_data_downloader/2011_09_28/2011_09_28_drive_0001_sync/velodyne_points/data/0000000000.bin";
    std::vector<PointXYZI> cloud = loadKittiBinFile(path);
    
    std::cout << "점 개수 : " << cloud.size() << std::endl;

    return app.exec();
}

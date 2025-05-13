#include "MainWindow.h"
#include "MyOpenGLWidget.h"
#include "BinLoader.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    setupUI();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    MyOpenGLWidget *glWidget = new MyOpenGLWidget;
    QPushButton *button = new QPushButton("눌러 봐요");

    QObject::connect(button, &QPushButton::clicked, []()
                     { qDebug() << "버튼이 눌러졌습니다!"; });

    layout->addWidget(glWidget, 1);
    layout->addWidget(button);

    QPushButton *openFolderBtn = new QPushButton("폴더 열기", glWidget);
    openFolderBtn->move(10, 10); // 좌상단 위치
    openFolderBtn->raise(); // 위로 올리기

    QObject::connect(openFolderBtn, &QPushButton::clicked, this, &MainWindow::onOpenFolderClicked);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}

void MainWindow::onOpenFolderClicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "폴더 선택");

    if (folderPath.isEmpty())
        return;

    QDir dir(folderPath);
    QStringList binFiles = dir.entryList(QStringList() << "*.bin", QDir::Files);

    for (const QString &fileName : binFiles)
    {
        std::string filePath = QDir(folderPath).filePath(fileName).toStdString();
        std::vector<PointXYZI> points = BinLoader::loadKittiBinFile(filePath);

        // 이후 points 사용: 예를 들어 저장, 시각화 등
        qDebug() << QString("로드 완료: %1, 포인트 수: %2")
                        .arg(fileName)
                        .arg(points.size());
    }
}
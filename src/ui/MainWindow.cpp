#include "MainWindow.h"
#include "BinLoader.h"
#include "OpenFolderButton.h"
#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    setupUI();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;

    glWidget_ = new MyOpenGLWidget(centralWidget);
    openFolderBtn_ = new OpenFolderButton(glWidget_);
    connect(openFolderBtn_, &OpenFolderButton::folderSelected, this, &MainWindow::loadFolderData);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(glWidget_);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}

void MainWindow::loadFolderData(const QString &folderPath)
{
    QDir dir(folderPath);
    chunckPoints_.clear();
    QStringList binFiles = dir.entryList(QStringList() << "*.bin", QDir::Files);

    for (const QString &fileName : binFiles)
    {
        std::string filePath = QDir(folderPath).filePath(fileName).toStdString();
        std::vector<PointXYZI> points = BinLoader::loadKittiBinFile(filePath);
        chunckPoints_.push_back(points);
        // qDebug() << QString("로드 완료: %1, 포인트 수: %2").arg(fileName).arg(points.size());
    }
    qDebug() << QString("전체 데이터 수: %1").arg(chunckPoints_.size());
}
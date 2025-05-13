#include "MainWindow.h"
#include "BinLoader.h"
#include "OpenFolderButton.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QCoreApplication>

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

    progressBar_ = new QProgressBar;
    progressBar_->setOrientation(Qt::Horizontal);
    progressBar_->setRange(0, 100);
    progressBar_->setValue(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(glWidget_);
    layout->addWidget(progressBar_);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}

void MainWindow::loadFolderData(const QString &folderPath)
{
    QDir dir(folderPath);
    chunckPoints_.clear();
    QStringList binFiles = dir.entryList(QStringList() << "*.bin", QDir::Files);

    int totalFiles = binFiles.size();
    progressBar_->setRange(0, totalFiles);
    progressBar_->setValue(0);

    for (int i = 0; i < totalFiles; ++i)
    {
        const QString &fileName = binFiles[i];
        std::string filePath = QDir(folderPath).filePath(fileName).toStdString();
        std::vector<PointXYZI> points = BinLoader::loadKittiBinFile(filePath);
        chunckPoints_.push_back(points);

        progressBar_->setValue(i + 1); // 현재 진행률 표시
        QCoreApplication::processEvents(); // UI 갱신을 강제로 수행
        // qDebug() << QString("로드 완료: %1, 포인트 수: %2").arg(fileName).arg(points.size());
    }
    
    emit dataLoaded(chunckPoints_);
    qDebug() << QString("전체 데이터 수: %1").arg(chunckPoints_.size());
}

#include "MainWindow.h"
#include "BinLoader.h"
#include "OpenFolderButton.h"
#include "ProgressDialog.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    setupUI();
    createConnection();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;

    glWidget_ = new MyOpenGLWidget(centralWidget);
    openFolderBtn_ = new OpenFolderButton(glWidget_);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(glWidget_);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}

void MainWindow::createConnection()
{
    connect(openFolderBtn_, &QPushButton::clicked, this, &MainWindow::onOpenFolderClikced);
}

void MainWindow::open()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder");
    if (folderPath.isEmpty())
        return;
    qDebug() << folderPath;
    loadFolderData(folderPath);
}

void MainWindow::onOpenFolderClikced()
{
    open();
}

void MainWindow::loadFolderData(const QString &folderPath)
{
    QDir dir(folderPath);
    chunckPoints_.clear();
    QStringList binFiles = dir.entryList(QStringList() << "*.bin", QDir::Files);
    int totalFiles = binFiles.size();

    ProgressDialog progressDialog(this);
    progressDialog.setRange(0, totalFiles);
    progressDialog.show();
    QCoreApplication::processEvents(); // ensure dialog is shown

    for (int i = 0; i < totalFiles; ++i)
    {
        const QString &fileName = binFiles[i];
        std::string filePath = QDir(folderPath).filePath(fileName).toStdString();
        std::vector<PointXYZI> points = BinLoader::loadKittiBinFile(filePath);
        chunckPoints_.push_back(points);

        progressDialog.setValue(i + 1);
        QCoreApplication::processEvents(); // ensure UI updates
        // qDebug() << QString("로드 완료: %1, 포인트 수: %2").arg(fileName).arg(points.size());
    }

    progressDialog.close();

    emit dataLoaded(chunckPoints_);
    qDebug() << QString("전체 데이터 수: %1").arg(chunckPoints_.size());
}

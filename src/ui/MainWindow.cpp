#include "MainWindow.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QCoreApplication>

#include "BinLoader.h"
#include "OpenFolderButton.h"
#include "ProgressDialog.h"
#include "BinDataLoader.h"

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

void MainWindow::loadFolderData(const QString &folderPath)
{
    QDir dir(folderPath);
    chunckPoints_.clear();
    QStringList binFiles = dir.entryList(QStringList() << "*.bin", QDir::Files);
    int totalFiles = binFiles.size();

    BinDataLoader loader;

    ProgressDialog progressDialog(this);
    progressDialog.setRange(0, totalFiles);
    progressDialog.show();
    QCoreApplication::processEvents(); // 해당 코드가 있어야, 제대로 동작

    connect(&loader, &BinDataLoader::progressUpdated, &progressDialog, &ProgressDialog::updateProgress);
    
    std::vector<std::vector<PointXYZI>> points = loader.loadFromFolder(folderPath);
    qDebug() << QString("points.size: %1").arg(points.size());

    progressDialog.close();
    emit dataLoaded(chunckPoints_);
}


void MainWindow::onOpenFolderClikced()
{
    open();
}
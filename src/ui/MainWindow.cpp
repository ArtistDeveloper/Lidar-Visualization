#include "MainWindow.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QCoreApplication>

#include "PointTypes.h"
#include "ProgressDialog.h"
#include "KittiBinDirectoryLoader.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    setupUI();
    mediator_ = new PlaybackMediator(openFolderBtn_, // NEW
                                     playerWidget_,
                                     glWidget_,
                                     this);
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;

    glWidget_ = new PointCloudViewer(centralWidget);
    openFolderBtn_ = new LoadFolderButton(glWidget_);
    playerWidget_ = new PointCloudPlayerWidget;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(glWidget_);
    layout->addWidget(playerWidget_);

    layout->setStretch(0, 8); // 첫 번째 위젯 (glWidget_)
    layout->setStretch(1, 1); // 두 번째 위젯 (playerWidget_)

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}

void MainWindow::onOpenFolderClikced()
{
    open();
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
    QStringList binFiles = dir.entryList(QStringList() << "*.bin", QDir::Files);
    int totalFiles = binFiles.size();

    KittiBinDirectoryLoader loader;

    ProgressDialog progressDialog(this);
    progressDialog.setRange(0, totalFiles);
    progressDialog.show();
    QCoreApplication::processEvents(); // 해당 코드가 있어야 Progress dialog가 제대로 동작

    connect(&loader, &KittiBinDirectoryLoader::progressUpdated, &progressDialog, &ProgressDialog::updateProgress);

    std::vector<std::vector<PointXYZI>> points = loader.loadFromFolder(folderPath);
    qDebug() << QString("points.size: %1").arg(points.size());

    progressDialog.close();

    player_->setEntireData(points);
    playerWidget_->setMaximum(points.size() - 1);
}


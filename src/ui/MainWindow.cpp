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
    // TODO: 에러 나지 않게 구현 추가 필요
    playerWidget_ = new PointCloudPlayerWidget;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(glWidget_);
    layout->addWidget(playerWidget_);

    // 비율 설정: glWidget 4, playerWidget 1
    layout->setStretch(0, 8); // 첫 번째 위젯 (glWidget_)
    layout->setStretch(1, 1); // 두 번째 위젯 (playerWidget_)

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}

void MainWindow::createConnection()
{
    player_ = new PointCloudPlayer;

    connect(openFolderBtn_, &QPushButton::clicked, this, &MainWindow::onOpenFolderClikced);

    // TODO: Frame Player 만들기 위한 이벤트 연결
    // connect(player_, &PointCloudPlayer::frameChanged, [=](int index)
    //         {
    // if (index < chunckPoints_.size())
    //     glWidget_->setPointCloudData(chunckPoints_[index]);
    // playerWidget_->updateSlider(index); });
    // connect(playerWidget_, &PointCloudPlayerWidget::onPlayPauseClicked, player_, &PointCloudPlayer::play);
    // connect(playerWidget_, &PointCloudPlayerWidget::pauseClicked, player_, &PointCloudPlayer::pause);
    // connect(playerWidget_, &PointCloudPlayerWidget::nextClicked, player_, &PointCloudPlayer::nextFrame);
    // connect(playerWidget_, &PointCloudPlayerWidget::prevClicked, player_, &PointCloudPlayer::prevFrame);
    // connect(playerWidget_, &PointCloudPlayerWidget::onSliderMoved, player_, &PointCloudPlayer::setFrame);
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

    // TODO: point 로드 후 player와 연결
    // chunckPoints_ = points;
    // player_->setData(chunckPoints_);
    // playerWidget_->setMaximum(chunckPoints_.size() - 1);
    emit dataLoaded(chunckPoints_);
}

void MainWindow::onOpenFolderClikced()
{
    open();
}
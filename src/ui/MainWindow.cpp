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

    glWidget_ = new PointCloudViewer(centralWidget);
    openFolderBtn_ = new OpenFolderButton(glWidget_);
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

void MainWindow::createConnection()
{
    player_ = new PointCloudPlayer;

    // 폴더 탐색 버튼 슬롯 연결
    connect(openFolderBtn_, &QPushButton::clicked, this, &MainWindow::onOpenFolderClikced);

    // PointCloudPlayerWidget의 시그널과 PointCloudPlayer의 슬롯 연결
    connect(playerWidget_, &PointCloudPlayerWidget::playClicked, player_, &PointCloudPlayer::play);
    connect(playerWidget_, &PointCloudPlayerWidget::pauseClicked, player_, &PointCloudPlayer::pause);
    connect(playerWidget_, &PointCloudPlayerWidget::nextClicked, player_, &PointCloudPlayer::nextFrame);
    connect(playerWidget_, &PointCloudPlayerWidget::prevClicked, player_, &PointCloudPlayer::prevFrame);
    connect(playerWidget_, &PointCloudPlayerWidget::sliderMoved, player_, &PointCloudPlayer::setFrame);

    // PointCloudPlayer와 glWidget의 연결
    connect(playerWidget_, &PointCloudPlayerWidget::sliderMoved, player_, &PointCloudPlayer::setFrame);
    connect(player_, &PointCloudPlayer::frameChanged, glWidget_, &PointCloudViewer::setPointCloudData);
    connect(player_, &PointCloudPlayer::frameIndexChanged, playerWidget_, &PointCloudPlayerWidget::updateSlider);
    connect(player_, &PointCloudPlayer::playbackStopped, playerWidget_, &PointCloudPlayerWidget::stopPlayback);
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

    BinDataLoader loader;

    ProgressDialog progressDialog(this);
    progressDialog.setRange(0, totalFiles);
    progressDialog.show();
    QCoreApplication::processEvents(); // 해당 코드가 있어야 Progress dialog가 제대로 동작

    connect(&loader, &BinDataLoader::progressUpdated, &progressDialog, &ProgressDialog::updateProgress);

    std::vector<std::vector<PointXYZI>> points = loader.loadFromFolder(folderPath);
    qDebug() << QString("points.size: %1").arg(points.size());

    progressDialog.close();

    player_->setEntireData(points);
    playerWidget_->setMaximum(points.size() - 1);
}

void MainWindow::onOpenFolderClikced()
{
    open();
}
/*!SECTION
가져야 할 책임:
1. UI 구성 (setupUI)
2. 시그널-슬롯 연결 (createConnection)
3. 사용자 인터랙션 처리 (onOpenFolderClicked)
4. 로직 클래스 호출 및 연결 (BinDataLoader 같은 로직 클래스 사용)
5. 데이터 로드 후 UI 전달 및 후처리 (emit dataLoaded, glWidget_에 전달 등)

가져서는 안 되는 책임:
1. 로우 레벨 I/O (BinLoader)
2. 데이터 파싱 및 처리 로직 (loadFolderData)
3. 긴 반복 작업 및 상태 관리 (progressDialog 직접 제어 포함)
*/

#include "MainWindow.h"
#include "BinLoader.h"
#include "OpenFolderButton.h"
#include "ProgressDialog.h"
#include "BinDataLoader.h"

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
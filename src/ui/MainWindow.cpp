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


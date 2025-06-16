#include "MainWindow.h"

#include <QMenu>
#include <QVBoxLayout>

#include "PlaybackMediator.h"
#include "PointCloudViewer.h"
#include "LoadFolderButton.h"
#include "PointCloudPlayerWidget.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    setupUI();
    setupMediator();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;

    glWidget_ = new PointCloudViewer(centralWidget);
    openFolderBtn_ = new LoadFolderButton(glWidget_);
    playerWidget_ = new PointCloudPlayerWidget;

    menuButton_ = new QPushButton(QIcon(":/assets/menu.png"), "", centralWidget);
    menuButton_->move(10, 10);
    menuButton_->setFixedSize(100, 100);
    menuButton_->setIconSize(QSize(100, 100));
    // menuButton_->setStyleSheet("QPushButton { border: none; background: transparent; }");
    // menuButton_->setText("메뉴");
    qDebug() << QIcon(":/assets/menu.png").isNull();


    QMenu* menu = new QMenu(this);
    QAction* openKittiAction = new QAction("Open Kitti Data Folder", this);
    QAction* openFieldAction = new QAction("Open Field Data", this);
    menu->addAction(openKittiAction);
    menu->addAction(openFieldAction);

    connect(menuButton_, &QPushButton::clicked, this, [=]() {
        menu->exec(menuButton_->mapToGlobal(QPoint(0, menuButton_->height())));
    });

    connect(openKittiAction, &QAction::triggered, openFolderBtn_, &LoadFolderButton::chooseFolder);
    connect(openFieldAction, &QAction::triggered, openFolderBtn_, &LoadFolderButton::chooseFieldData);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(glWidget_);
    layout->addWidget(playerWidget_);

    layout->setStretch(0, 8); // 첫 번째 위젯 (glWidget_)
    layout->setStretch(1, 1); // 두 번째 위젯 (playerWidget_)

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}

void MainWindow::setupMediator()
{
    mediator_ = new PlaybackMediator(openFolderBtn_,  // 그대로 유지
                                     playerWidget_,
                                     glWidget_,
                                     this);
}

#include "MainWindow.h"

#include <QMenu>
#include <QVBoxLayout>

#include "PlaybackMediator.h"
#include "PointCloudViewer.h"
#include "PointCloudPlayerWidget.h"
#include "MenuButton.h"

MainWindow::MainWindow(std::shared_ptr<PointCloudRepository> repo, QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags), repo_(std::move(repo))
{
    setupUI();
    setupMediator();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;

    glWidget_ = new PointCloudViewer(centralWidget);
    playerWidget_ = new PointCloudPlayerWidget;
    menuBtn_ = new MenuButton(centralWidget);

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
    mediator_ = new PlaybackMediator(menuBtn_,  // 그대로 유지
                                     playerWidget_,
                                     glWidget_,
                                     repo_,
                                     this);
}

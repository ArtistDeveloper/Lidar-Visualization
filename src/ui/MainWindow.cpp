#include "MainWindow.h"
#include "MyOpenGLWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    setupUI();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    MyOpenGLWidget *glWidget = new MyOpenGLWidget;
    QPushButton *button = new QPushButton("눌러 봐요");

    QObject::connect(button, &QPushButton::clicked, []()
                     { qDebug() << "버튼이 눌러졌습니다!"; });

    layout->addWidget(glWidget, 1);
    layout->addWidget(button);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}
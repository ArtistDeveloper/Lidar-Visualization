#pragma once

#include <QMainWindow>
#include <QPushButton>

#include "OpenFolderButton.h"
#include "MyOpenGLWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

private slots:
    void loadFolderData(const QString &folderPath);

private:
    void setupUI();
    MyOpenGLWidget *glWidget_;
    OpenFolderButton *openFolderBtn_;
    std::vector<std::vector<PointXYZI>> chunckPoints_;
};
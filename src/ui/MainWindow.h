#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>

#include "OpenFolderButton.h"
#include "MyOpenGLWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

private slots:
    void loadFolderData(const QString &folderPath);

signals:
    void dataLoaded(const std::vector<std::vector<PointXYZI>> &data);

private:
    void setupUI();
    MyOpenGLWidget *glWidget_;
    OpenFolderButton *openFolderBtn_;
    std::vector<std::vector<PointXYZI>> chunckPoints_;
    QProgressBar *progressBar_;
};
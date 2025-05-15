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

private:
    void setupUI();
    void createConnection();
    void open();
    void loadFolderData(const QString &folderPath);

    MyOpenGLWidget *glWidget_;
    OpenFolderButton *openFolderBtn_;
    std::vector<std::vector<PointXYZI>> chunckPoints_;

signals:
    void dataLoaded(const std::vector<std::vector<PointXYZI>> &data);

private slots:
    void onOpenFolderClikced();
};
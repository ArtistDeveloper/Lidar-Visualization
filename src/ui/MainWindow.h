#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>

#include "PointTypes.h"

class LoadFolderButton;
class PointCloudViewer;
class PointCloudPlayer;
class PointCloudPlayerWidget;
class PlaybackMediator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

private:
    void setupUI();
    void setupMediator();

    PointCloudViewer *glWidget_;
    LoadFolderButton *openFolderBtn_;
    PointCloudPlayerWidget *playerWidget_;
    PlaybackMediator *mediator_;

signals:
    void dataLoaded(const std::vector<std::vector<PointXYZI>> &data);

private slots:
};
#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>

#include "PointTypes.h"

class PointCloudViewer;
class PointCloudPlayer;
class PointCloudPlayerWidget;
class PlaybackMediator;
class MenuButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

private:
    void setupUI();
    void setupMediator();

    PointCloudViewer *glWidget_;
    PointCloudPlayerWidget *playerWidget_;
    PlaybackMediator *mediator_;
    MenuButton* menuBtn_;

signals:
    void dataLoaded(const std::vector<std::vector<PointXYZI>> &data);

private slots:
};

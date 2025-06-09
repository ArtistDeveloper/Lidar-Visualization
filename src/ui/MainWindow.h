#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>

#include "LoadFolderButton.h"
#include "PointCloudViewer.h"
#include "PointCloudPlayer.h"
#include "PointCloudPlayerWidget.h"
#include "PlaybackMediator.h"

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

    PointCloudViewer *glWidget_;
    LoadFolderButton *openFolderBtn_;
    PointCloudPlayer *player_;
    PointCloudPlayerWidget *playerWidget_;
    PlaybackMediator *mediator_;

signals:
    void dataLoaded(const std::vector<std::vector<PointXYZI>> &data);

private slots:
    void onOpenFolderClikced();
};
#pragma once

#include <QObject>
#include <QPointer>

#include "PointTypes.h"

class PointCloudPlayerWidget;
class PointCloudPlayer;
class PointCloudViewer;
class LoadFolderController;
class MenuButton;

class PlaybackMediator : public QObject
{
    Q_OBJECT
public:
    PlaybackMediator(MenuButton *openBtn,
                     PointCloudPlayerWidget *controls,
                     PointCloudViewer *viewer,
                     QObject *parent = nullptr);

private slots:
    // UI에서 온 요청
    void onPlay();
    void onPause();
    void onNext();
    void onPrev();
    void onSliderMoved(int);

    // Player에서 온 알림
    void onFrameChanged(const std::vector<PointXYZI> &);
    void onFrameIndexChanged(int);
    void onPlaybackStopped();

    // 데이터 로더 → Mediator
    void onDataLoaded(const std::vector<std::vector<PointXYZI>> &data);

private:
    void loadFolder(const QString &dir);

    MenuButton *menuBtn_;
    PointCloudPlayerWidget *controls_;
    PointCloudViewer *viewer_;
    std::unique_ptr<PointCloudPlayer> player_;
    std::unique_ptr<LoadFolderController> loaderCtl_;
};

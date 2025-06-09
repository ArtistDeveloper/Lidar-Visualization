#pragma once

#include <QObject>
#include <QPointer>

#include "PointTypes.h"

class LoadFolderButton;
class PointCloudPlayerWidget;
class PointCloudPlayer;
class PointCloudViewer;

class PlaybackMediator : public QObject
{
    Q_OBJECT
public:
    PlaybackMediator(LoadFolderButton* openBtn,
                     PointCloudPlayerWidget* controls,
                     PointCloudViewer* viewer,
                     QObject* parent = nullptr);

private slots:
    // UI에서 온 요청
    void onOpenFolderClicked();
    void onPlay();
    void onPause();
    void onNext();
    void onPrev();
    void onSliderMoved(int);

    // Player에서 온 알림
    void onFrameChanged(const std::vector<PointXYZI>&);
    void onFrameIndexChanged(int);
    void onPlaybackStopped();

private:
    void loadFolder(const QString& dir);

    QPointer<LoadFolderButton>     openBtn_;
    QPointer<PointCloudPlayerWidget> controls_;
    QPointer<PointCloudViewer>     viewer_;
    std::unique_ptr<PointCloudPlayer> player_;
};

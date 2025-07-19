/*
Player와 Widget 간의 양방향 이벤트를 중재하는 허브용도로 사용
*/

#include "PlaybackMediator.h"

#include <QFileDialog>
#include <QDir>
#include <QCoreApplication>

#include "MenuButton.h"
#include "PointCloudPlayerWidget.h"
#include "PointCloudViewer.h"
#include "PointCloudPlayer.h"
#include "ProgressDialog.h"
#include "KittiBinDirectoryLoader.h"
#include "LoadFolderController.h"
#include "PointCloudRepository.h"

#ifndef MAX_GRID_SIZE
#define MAX_GRID_SIZE 500
#endif

extern void buildGridForFrame(const std::vector<PointXYZI> &pts);  // main.cpp 내부 함수
extern std::vector<int> gridIndices[MAX_GRID_SIZE][MAX_GRID_SIZE]; // 전역 인덱스
extern void buildGroundSegmentedAggregate(
    const std::vector<PointXYZI> &inPts,
    std::vector<PointXYZI> &out);

PlaybackMediator::PlaybackMediator(MenuButton *openBtn,
                                   PointCloudPlayerWidget *controls,
                                   PointCloudViewer *viewer,
                                   std::shared_ptr<PointCloudRepository> repo,
                                   QObject *parent)
    : QObject(parent),
      menuBtn_(openBtn), controls_(controls), viewer_(viewer),
      repo_(std::move(repo)),
      player_(std::make_unique<PointCloudPlayer>()),
      loaderCtl_(std::make_unique<LoadFolderController>(this))
{
    // Folder 선택 → Loader
    connect(menuBtn_, &MenuButton::folderSelected, loaderCtl_.get(), &LoadFolderController::load);

    // Loader 완료 → Mediator
    connect(loaderCtl_.get(), &LoadFolderController::finished, this, &PlaybackMediator::onDataLoaded);

    // UI가 Mediator에게 변화를 알려주는 이벤트를 연결한다
    connect(controls_, &PointCloudPlayerWidget::playClicked, this, &PlaybackMediator::onPlay);
    connect(controls_, &PointCloudPlayerWidget::pauseClicked, this, &PlaybackMediator::onPause);
    connect(controls_, &PointCloudPlayerWidget::nextClicked, this, &PlaybackMediator::onNext);
    connect(controls_, &PointCloudPlayerWidget::prevClicked, this, &PlaybackMediator::onPrev);
    connect(controls_, &PointCloudPlayerWidget::sliderMoved, this, &PlaybackMediator::onSliderMoved);

    // Player의 동작을 Mediator에 알리며, Mediator는 실제 동작을 선택
    connect(player_.get(), &PointCloudPlayer::frameChanged, this, &PlaybackMediator::onFrameChanged);
    connect(player_.get(), &PointCloudPlayer::frameIndexChanged, this, &PlaybackMediator::onFrameIndexChanged);
    connect(player_.get(), &PointCloudPlayer::playbackStopped, this, &PlaybackMediator::onPlaybackStopped);
}

/* ---------- UI 이벤트 (controls) ---------- */

void PlaybackMediator::onPlay()
{
    player_->play();
}

void PlaybackMediator::onPause()
{
    player_->pause();
}

void PlaybackMediator::onNext()
{
    player_->nextFrame();
}

void PlaybackMediator::onPrev()
{
    player_->prevFrame();
}

void PlaybackMediator::onSliderMoved(int i)
{
    player_->setFrame(i);
}

/* ---------- Player 알림 → UI ---------- */

// void PlaybackMediator::onFrameChanged(const std::vector<PointXYZI> &pts)
// {
//     viewer_->setPointCloudData(pts);
// }

// NOTE: Checkboard Test Rendering
// void PlaybackMediator::onFrameChanged(const std::vector<PointXYZI> &pts)
// {
//     buildGridForFrame(pts);

//     static std::vector<PointXYZI> aggregate;
//     aggregate.clear();
//     aggregate.reserve(pts.size()); // 전체 크기만큼 미리 예약

//     for (int gx = 0; gx < MAX_GRID_SIZE; ++gx)
//         for (int gy = 0; gy < MAX_GRID_SIZE; ++gy)
//         {
//             bool red = ((gx + gy) & 1); // true → 빨강
//             const auto &ids = gridIndices[gx][gy];
//             for (int id : ids)
//             {
//                 PointXYZI p = pts[id]; // 원본 복사
//                 p.intensity = red ? 1.0f : 0.0f; // 플래그만 덮어씀
//                 aggregate.push_back(p);
//             }
//         }

//     viewer_->setPointCloudData(aggregate);
// }

void PlaybackMediator::onFrameChanged(const std::vector<PointXYZI> &pts)
{
    static std::vector<PointXYZI> aggregate;
    buildGroundSegmentedAggregate(pts, aggregate);
    viewer_->setPointCloudData(aggregate);
}

void PlaybackMediator::onFrameIndexChanged(int idx)
{
    controls_->updateSlider(idx);
}

void PlaybackMediator::onPlaybackStopped()
{
    controls_->stopPlayback();
}

/* ---------- 데이터 로드 결과 ---------- */

void PlaybackMediator::onDataLoaded(std::shared_ptr<std::vector<std::vector<PointXYZI>>> data)
{
    if (!data || data->empty())
        return;

    repo_->setPointCloudData(std::move(data)); // 레포지토리에 데이터 삽입
    player_->setDataSource(repo_->dataPtr());  // 같은 포인터를 Player에 전달
    controls_->setMaximum(player_->totalFrames() - 1);
}
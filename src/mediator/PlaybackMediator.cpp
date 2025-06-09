/*
Player와 Widget 간의 양방향 이벤트를 중재하는 허브용도로 사용
*/ 

#include "PlaybackMediator.h"

#include <QFileDialog>
#include <QDir>
#include <QCoreApplication>

#include "LoadFolderButton.h"
#include "PointCloudPlayerWidget.h"
#include "PointCloudViewer.h"
#include "PointCloudPlayer.h"
#include "ProgressDialog.h"
#include "KittiBinDirectoryLoader.h"

PlaybackMediator::PlaybackMediator(LoadFolderButton *openBtn,
                                   PointCloudPlayerWidget *controls,
                                   PointCloudViewer *viewer,
                                   QObject *parent)
    : QObject(parent),
      openBtn_(openBtn), controls_(controls), viewer_(viewer),
      player_(std::make_unique<PointCloudPlayer>())
{
    // 1. UI가 Mediator에게 변화를 알려주는 이벤트를 연결한다
    connect(openBtn_, &QPushButton::clicked, this, &PlaybackMediator::onOpenFolderClicked);
    connect(controls_, &PointCloudPlayerWidget::playClicked, this, &PlaybackMediator::onPlay);
    connect(controls_, &PointCloudPlayerWidget::pauseClicked, this, &PlaybackMediator::onPause);
    connect(controls_, &PointCloudPlayerWidget::nextClicked, this, &PlaybackMediator::onNext);
    connect(controls_, &PointCloudPlayerWidget::prevClicked, this, &PlaybackMediator::onPrev);
    connect(controls_, &PointCloudPlayerWidget::sliderMoved, this, &PlaybackMediator::onSliderMoved);

    // 2. Player의 동작을 Mediator에 알리며, Mediator는 실제 동작을 선택
    connect(player_.get(), &PointCloudPlayer::frameChanged, this, &PlaybackMediator::onFrameChanged);
    connect(player_.get(), &PointCloudPlayer::frameIndexChanged, this, &PlaybackMediator::onFrameIndexChanged);
    connect(player_.get(), &PointCloudPlayer::playbackStopped, this, &PlaybackMediator::onPlaybackStopped);
}

/* ---------- UI 이벤트 ---------- */

void PlaybackMediator::onOpenFolderClicked()
{
    const QString dir = QFileDialog::getExistingDirectory(nullptr, "Select Folder");
    if (!dir.isEmpty())
        loadFolder(dir);
}

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

void PlaybackMediator::onFrameChanged(const std::vector<PointXYZI> &pts)
{
    viewer_->setPointCloudData(pts);
}

void PlaybackMediator::onFrameIndexChanged(int idx)
{
    controls_->updateSlider(idx);
}

void PlaybackMediator::onPlaybackStopped()
{
    controls_->stopPlayback();
}

/* ---------- 폴더 로드 ---------- */

void PlaybackMediator::loadFolder(const QString &folder)
{
    QDir dir(folder);
    const QStringList bin = dir.entryList(QStringList() << "*.bin", QDir::Files);
    const int total = bin.size();

    KittiBinDirectoryLoader loader;
    ProgressDialog dlg;
    dlg.setRange(0, total);
    dlg.show();
    connect(&loader, &KittiBinDirectoryLoader::progressUpdated,
            &dlg, &ProgressDialog::updateProgress);
    QCoreApplication::processEvents();

    auto all = loader.loadFromFolder(folder);
    dlg.close();

    player_->setEntireData(all);
    controls_->setMaximum(static_cast<int>(all.size()) - 1);
}

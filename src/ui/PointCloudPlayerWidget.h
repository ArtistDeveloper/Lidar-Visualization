// PointCloudPlayerWidget.h
#pragma once

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

class PointCloudPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointCloudPlayerWidget(QWidget *parent = nullptr);
    void setMaximum(int maxFrameIndex); // 슬라이더 최대값 설정

// Signal을 사용하여 UI가 동작하는 알림만 전달하고, 실제 비즈니스 로직은 PointCloudPlayer.cpp에 구현
signals:
    void playClicked();
    void pauseClicked();
    void nextClicked();
    void prevClicked();
    void sliderMoved(int index);

public slots:
    void updateSlider(int index); // 외부에서 현재 프레임을 반영

private slots:
    void onPlayPauseClicked();
    void onSliderMoved(int value);
    void onPrevClicked();
    void onNextClicked();

private:
    void setupUI();
    void createConnection();

    QLabel *currentFrameLabel_;
    QLabel *maxFrameLabel_;
    QPushButton *playPauseBtn_;
    QPushButton *nextBtn_;
    QPushButton *prevBtn_;
    QSlider *slider_;
    bool isPlaying_ = false;
};

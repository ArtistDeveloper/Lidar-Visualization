// PointCloudPlayerWidget.h
#pragma once

#include <QWidget>
#include <QPushButton>
#include <QSlider>

class PointCloudPlayerWidget : public QWidget {
    Q_OBJECT

public:
    explicit PointCloudPlayerWidget(QWidget *parent = nullptr);
    void setMaximum(int max);

signals:
    void onPlayPauseClicked();
    void pauseClicked();
    void nextClicked();
    void prevClicked();
    void onSliderMoved(int index);

public slots:
    void updateSlider(int index);

private:
    void setupUI();
    void createConnection();

    QPushButton *playPauseBtn_;
    QPushButton *nextBtn_;
    QPushButton *prevBtn_;
    QSlider *slider_;
    bool isPlaying_ = false;
};

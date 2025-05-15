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
    void playClicked();
    void pauseClicked();
    void nextClicked();
    void prevClicked();
    void sliderMoved(int index);

public slots:
    void updateSlider(int index);

private:
    QPushButton *playButton_;
    QPushButton *nextButton_;
    QPushButton *prevButton_;
    QSlider *slider_;
    bool isPlaying_ = false;
};

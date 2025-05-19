#pragma once

#include <QObject>
#include <QTimer>

#include "BinLoader.h"

class PointCloudPlayer : public QObject
{
    Q_OBJECT

public:
    explicit PointCloudPlayer(QObject *parent = nullptr);
    void setEntireData(const std::vector<std::vector<PointXYZI>> &data);
    void play();
    void pause();
    void nextFrame();
    void prevFrame();
    void setFrame(int index);

    int currentFrame() const;
    int totalFrames() const;

signals:
    void frameChanged(int index);

private slots:
    void onTimeout();

private:
    std::vector<std::vector<PointXYZI>> data_;
    int currentIndex_ = 0;
    QTimer timer_;
};

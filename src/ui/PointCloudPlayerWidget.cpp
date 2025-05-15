#include "PointCloudPlayerWidget.h"

#include <QHBoxLayout>

PointCloudPlayerWidget::PointCloudPlayerWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
    createConnection();
}

void PointCloudPlayerWidget::setupUI()
{
    playPauseBtn_ = new QPushButton("Play");
    prevBtn_ = new QPushButton("Prev");
    nextBtn_ = new QPushButton("Next");
    slider_ = new QSlider(Qt::Horizontal);

    slider_->setMinimum(0);
    slider_->setTickPosition(QSlider::TicksBelow);
    slider_->setSingleStep(1);

    auto layout = new QHBoxLayout;
    layout->addWidget(prevBtn_);
    layout->addWidget(playPauseBtn_);
    layout->addWidget(nextBtn_);
    layout->addWidget(slider_);
    setLayout(layout);
}

void PointCloudPlayerWidget::createConnection()
{
    connect(playPauseBtn_, &QPushButton::clicked, this, &PointCloudPlayerWidget::onPlayPauseClicked);
    connect(prevBtn_, &QPushButton::clicked, this, &PointCloudPlayerWidget::prevClicked);
    connect(nextBtn_, &QPushButton::clicked, this, &PointCloudPlayerWidget::nextClicked);
    connect(slider_, &QSlider::sliderMoved, this, &PointCloudPlayerWidget::onSliderMoved);
}

void PointCloudPlayerWidget::onPlayPauseClicked()
{
    isPlaying_ = !isPlaying_;
    playPauseBtn_->setText(isPlaying_ ? "Pause" : "Play");
    if (isPlaying_)
        emit playClicked();
    else
        emit pauseClicked();
}

void PointCloudPlayerWidget::onSliderMoved(int value)
{
    emit sliderMoved(value);
}

void PointCloudPlayerWidget::updateSlider(int index)
{
    slider_->setValue(index);
}

void PointCloudPlayerWidget::setMaximum(int maxFrameIndex)
{
    slider_->setMaximum(maxFrameIndex);
}
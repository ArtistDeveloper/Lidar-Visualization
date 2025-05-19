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

    currentFrameLabel_ = new QLabel("0");
    maxFrameLabel_ = new QLabel("0");

    slider_->setMinimum(0);
    slider_->setTickPosition(QSlider::TicksBelow);
    slider_->setSingleStep(1);
    slider_->setTickPosition(QSlider::NoTicks);

    auto layout = new QHBoxLayout;
    layout->addWidget(prevBtn_);
    layout->addWidget(playPauseBtn_);
    layout->addWidget(nextBtn_);
    layout->addWidget(currentFrameLabel_);
    layout->addWidget(slider_);
    layout->addWidget(maxFrameLabel_);
    setLayout(layout);
}

void PointCloudPlayerWidget::createConnection()
{
    connect(playPauseBtn_, &QPushButton::clicked, this, &PointCloudPlayerWidget::onPlayPauseClicked);
    connect(prevBtn_, &QPushButton::clicked, this, &PointCloudPlayerWidget::onPrevClicked);
    connect(nextBtn_, &QPushButton::clicked, this, &PointCloudPlayerWidget::onNextClicked);

    connect(slider_, &QSlider::sliderMoved, this, &PointCloudPlayerWidget::onSliderMoved);
    connect(slider_, &QSlider::valueChanged, this, &PointCloudPlayerWidget::onSliderValueChanged);
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

void PointCloudPlayerWidget::onPrevClicked()
{
    emit prevClicked();
}

void PointCloudPlayerWidget::onNextClicked()
{
    emit nextClicked();
}

void PointCloudPlayerWidget::onSliderMoved(int value)
{
    emit sliderMoved(value); // 이게 player_->setFrame(value)로 연결됨
}

void PointCloudPlayerWidget::onSliderValueChanged(int value)
{
    if (!slider_->isSliderDown())
    {
        emit sliderMoved(value);
    }
}

void PointCloudPlayerWidget::updateSlider(int index)
{
    slider_->setValue(index);
    currentFrameLabel_->setText(QString::number(index));
}

void PointCloudPlayerWidget::setMaximum(int maxFrameIndex)
{
    slider_->setMaximum(maxFrameIndex);
    maxFrameLabel_->setText(QString::number(maxFrameIndex));
}
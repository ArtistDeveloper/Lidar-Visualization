#include "PointCloudPlayerWidget.h"

#include <QHBoxLayout>

PointCloudPlayerWidget::PointCloudPlayerWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
    createConnection();
}

void PointCloudPlayerWidget::setMaximum(int max)
{
    throw std::logic_error("setMaximum() is not implemented yet.");
}

void PointCloudPlayerWidget::updateSlider(int index)
{
    throw std::logic_error("updateSlider() is not implemented yet.");
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
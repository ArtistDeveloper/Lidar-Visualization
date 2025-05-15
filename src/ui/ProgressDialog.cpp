#include "ProgressDialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Loading...");
    setModal(true);
    setFixedSize(300, 80);

    progressBar_ = new QProgressBar(this);
    progressBar_->setOrientation(Qt::Horizontal);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(progressBar_);
    setLayout(layout);
}

void ProgressDialog::setRange(int min, int max)
{
    progressBar_->setRange(min, max);
}

void ProgressDialog::updateProgress(int value)
{
    progressBar_->setValue(value);
}
#pragma once

#include <QDialog>
#include <QProgressBar>
#include <QVBoxLayout>

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = nullptr);

    void setRange(int min, int max);

public slots:
    void updateProgress(int value);

private:
    QProgressBar *progressBar_;

signals:
    void cancelled();
};
#pragma once

#include <QPushButton>

class OpenFolderButton : public QPushButton
{
    Q_OBJECT

public:
    explicit OpenFolderButton(QWidget *parent = nullptr);

signals:
    void folderSelected(const QString &folderPath);

private slots:
    void handleClick();
};
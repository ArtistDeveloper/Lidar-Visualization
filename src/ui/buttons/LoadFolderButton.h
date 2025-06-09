#pragma once

#include <QPushButton>

class LoadFolderButton : public QPushButton
{
    Q_OBJECT

public:
    explicit LoadFolderButton(QWidget *parent = nullptr);

signals:
    void folderSelected(const QString &path);

private slots:
    void chooseFolder();
};
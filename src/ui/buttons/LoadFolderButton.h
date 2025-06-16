#pragma once

#include <QPushButton>

class LoadFolderButton : public QPushButton
{
    Q_OBJECT

public:
    explicit LoadFolderButton(QWidget *parent = nullptr);

signals:
    void folderSelected(const QString &path);
    void fieldFileSelected(const QString &filePath);

public slots:
    void chooseFolder(); 
    void chooseFieldData();
};

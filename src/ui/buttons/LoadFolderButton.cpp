#include "LoadFolderButton.h"

#include <QFileDialog>
#include <QDebug>

LoadFolderButton::LoadFolderButton(QWidget *parent) : QPushButton("Open Folder", parent)
{
    move(10, 10);
    raise();
    connect(this, &QPushButton::clicked, this, &LoadFolderButton::chooseFolder);
}

void LoadFolderButton::chooseFolder()
{
    const QString dir = QFileDialog::getExistingDirectory(nullptr, "Select Folder");
    if (!dir.isEmpty())
        emit folderSelected(dir);
}
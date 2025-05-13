#include "OpenFolderButton.h"
#include <QFileDialog>

OpenFolderButton::OpenFolderButton(QWidget *parent) : QPushButton("폴더 열기", parent)
{
    move(10, 10);
    raise();
    connect(this, &QPushButton::clicked, this, &OpenFolderButton::handleClick);
}

void OpenFolderButton::handleClick()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "폴더 선택");
    if (folderPath.isEmpty())
    {
        emit folderSelected(folderPath);
    }
}
#include "LoadFolderButton.h"

#include <QFileDialog>
#include <QDebug>

LoadFolderButton::LoadFolderButton(QWidget *parent) : QPushButton("Open Folder", parent)
{
    move(10, 10);
    raise();
}
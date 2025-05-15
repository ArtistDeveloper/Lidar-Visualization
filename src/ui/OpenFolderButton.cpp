#include "OpenFolderButton.h"
#include <QFileDialog>
#include <QDebug>

OpenFolderButton::OpenFolderButton(QWidget *parent) : QPushButton("Open Folder", parent)
{
    move(10, 10);
    raise();
}
#include "MenuButton.h"

#include <QFileDialog>

MenuButton::MenuButton(QWidget *parent) : QPushButton(QIcon(":/assets/buttons/menu.png"), "", parent)
{
    move(10, 10);
    setFixedSize(100, 100);
    setIconSize(QSize(100, 100));
    // setStyleSheet("QPushButton { border: none; background: transparent; }");
    // setText("메뉴");

    menu = new QMenu(parent);
    QAction *openKittiAction = new QAction("Open Kitti Data Folder", this);
    QAction *openFieldAction = new QAction("Open Field Data", this);
    menu->addAction(openKittiAction);
    menu->addAction(openFieldAction);

    connect(this, &QPushButton::clicked, this, &MenuButton::pushMenu);
    connect(openKittiAction, &QAction::triggered, this, &MenuButton::chooseFolder);
    connect(openFieldAction, &QAction::triggered, this, &MenuButton::chooseFieldData);
}

void MenuButton::pushMenu()
{
    menu->exec(this->mapToGlobal(QPoint(0, this->height())));
}

void MenuButton::chooseFolder()
{
    const QString dir = QFileDialog::getExistingDirectory(nullptr, "Select Folder");
    if (!dir.isEmpty())
        emit folderSelected(dir);
}

void MenuButton::chooseFieldData() {
    const QString filePath = QFileDialog::getOpenFileName(nullptr, "Select .bin File", "", "*.bin");
    if (!filePath.isEmpty())
        emit fieldFileSelected(filePath);
}

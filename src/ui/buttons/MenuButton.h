#pragma once

#include <QPushButton>
#include <QMenu>

class MenuButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MenuButton(QWidget *parent = nullptr);

signals:
    void folderSelected(const QString &path);
    void fieldFileSelected(const QString &filePath);

public slots:
    void pushMenu();
    void chooseFolder();
    void chooseFieldData();

private:
    QMenu *menu;
};
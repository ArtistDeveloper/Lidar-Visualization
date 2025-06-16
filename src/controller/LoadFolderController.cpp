// src/controller/LoadFolderController.cpp
#include "LoadFolderController.h"
#include "ProgressDialog.h"
#include "KittiBinDirectoryLoader.h"
#include "CustomBinFileLoader.h"

#include <QDir>
#include <QCoreApplication>

LoadFolderController::LoadFolderController(QObject* parent)
    : QObject(parent)
{}

void LoadFolderController::showProgressDialog(int total)
{
    progressDlg_ = new ProgressDialog;
    progressDlg_->setRange(0, total);
    progressDlg_->show();
}

void LoadFolderController::load(const QString& folder)
{
    QDir dir(folder);
    const QStringList bin = dir.entryList(QStringList() << "*.bin", QDir::Files);
    const int total = bin.size();

    KittiBinDirectoryLoader loader;
    showProgressDialog(total);

    connect(&loader, &KittiBinDirectoryLoader::progressUpdated,
            progressDlg_, &ProgressDialog::updateProgress);

    QCoreApplication::processEvents();

    auto data = loader.loadFromFolder(folder);

    progressDlg_->close();
    progressDlg_->deleteLater();
    progressDlg_ = nullptr;

    emit finished(data);
}

void LoadFolderController::loadFieldData(const QString& filePath)
{
    CustomBinFileLoader loader;

    showProgressDialog(1);
    QCoreApplication::processEvents();

    auto data = loader.loadFromFile(filePath);  // 아직 구현 안 된 부분

    progressDlg_->close();
    progressDlg_->deleteLater();
    progressDlg_ = nullptr;

    emit finished(data);
}

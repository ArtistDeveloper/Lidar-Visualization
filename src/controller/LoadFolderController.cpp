#include "LoadFolderController.h"

#include <QDir>
#include <QCoreApplication>

#include "ProgressDialog.h"
#include "KittiBinDirectoryLoader.h"
#include "CustomBinFileLoader.h"

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

    connect(&loader, &KittiBinDirectoryLoader::progressUpdated, progressDlg_, &ProgressDialog::updateProgress);
    std::vector<std::vector<PointXYZI>> vec = loader.loadFromFolder(folder); // NRVO 수행

    progressDlg_->close();
    progressDlg_->deleteLater();
    progressDlg_ = nullptr;

    // vec -> shared_ptr o(1). 내부 포인트 데이터, 이중 벡터의 각 프레임 데이터 그대로 유지.
    // 실제 벡터 데이터는 힙에 존재
    std::shared_ptr<std::vector<std::vector<PointXYZI>>> data = std::make_shared<std::vector<std::vector<PointXYZI>>>(std::move(vec));

    emit finished(data);
}

void LoadFolderController::loadFieldData(const QString& filePath)
{
    CustomBinFileLoader loader;

    showProgressDialog(1);
    QCoreApplication::processEvents();

    std::vector<std::vector<PointXYZI>> vec = loader.loadFromFile(filePath);  // 아직 구현 안 된 부분

    progressDlg_->close();
    progressDlg_->deleteLater();
    progressDlg_ = nullptr;

    std::shared_ptr<std::vector<std::vector<PointXYZI>>> data = std::make_shared<std::vector<std::vector<PointXYZI>>>(std::move(vec));

    emit finished(data);
}

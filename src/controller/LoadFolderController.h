#pragma once
#include <QObject>
#include <vector>
#include "PointTypes.h"

class ProgressDialog;

class LoadFolderController : public QObject
{
    Q_OBJECT
public:
    explicit LoadFolderController(QObject* parent = nullptr);

public slots:
    void load(const QString& folder);
    void loadFieldData(const QString& filePath);

signals:
    void finished(std::shared_ptr<std::vector<std::vector<PointXYZI>>> data);

private:
    void showProgressDialog(int total);
    ProgressDialog* progressDlg_{nullptr};
};

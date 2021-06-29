#pragma once

#include <QWidget>

class QDirModel;
class QHBoxLayout;
class QTreeView;

class FilesystemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilesystemWidget(QWidget *parent = nullptr);

Q_SIGNALS:
    void urlFileDoubleClicked(const QString& filePath);

private:
    QHBoxLayout *m_mainLayout;
    QTreeView* m_filesystemTreeView;
    QDirModel* m_filesystemModel;
};

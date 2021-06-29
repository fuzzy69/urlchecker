#include "filesystemwidget.h"

#include <QDirModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QDebug>

FilesystemWidget::FilesystemWidget(QWidget *parent) : QWidget(parent)
{
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_filesystemModel = new QDirModel;
    m_filesystemTreeView = new QTreeView;
    m_filesystemTreeView->setModel(m_filesystemModel);
//    m_filesystemTreeView->resizeColumnToContents(0);
    m_mainLayout->addWidget(m_filesystemTreeView);
    //
    m_filesystemModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
//    m_filesystemModel->index(QString("/mnt/ramdisk/"));
    //
    connect(m_filesystemTreeView, &QTreeView::doubleClicked, [this](const QModelIndex& modelIndex){
        Q_UNUSED(modelIndex)
        QModelIndex index = m_filesystemTreeView->currentIndex();
        QFileInfo indexFileInfo = m_filesystemModel->fileInfo(index);
        if (indexFileInfo.isFile())
        {
            if (indexFileInfo.fileName().endsWith(".txt"))
                Q_EMIT urlFileDoubleClicked(indexFileInfo.absoluteFilePath());
        }
    });
}

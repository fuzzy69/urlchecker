#include "filesystemwidget.h"

#include <QDir>
#include <QFileSystemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QDebug>

FilesystemWidget::FilesystemWidget(QWidget *parent) : QWidget(parent)
{
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_filesystemModel = new QFileSystemModel;
    m_filesystemTreeView = new QTreeView;
    m_filesystemTreeView->setModel(m_filesystemModel);
    m_filesystemTreeView->hideColumn(1);
    m_filesystemTreeView->hideColumn(2);
    m_filesystemTreeView->hideColumn(3);
    m_mainLayout->addWidget(m_filesystemTreeView);
    m_filesystemModel->setRootPath(QString("/"));
    m_filesystemTreeView->setRootIndex(m_filesystemModel->index(QString("/")));
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

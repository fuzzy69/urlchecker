#include <QAction>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QString>

#include "recentfiles.h"


RecentFiles::RecentFiles(int maxRecentFiles, QObject* parent) : QObject(parent), m_maxRecentFiles(maxRecentFiles), m_recentFilesCount(0)
{
    m_recentFiles = QList<QString>();
    m_recentFilesActions = QList<QAction*>();
    for (int i = 0; i < m_maxRecentFiles; ++i)
    {
        QString filePath("");
        m_recentFiles.append(filePath);
        auto action = new QAction(filePath, this);
        action->setVisible(false);
        connect(action, &QAction::triggered, [action, this] {
            emit filePathSelected(action->text());
        });
        m_recentFilesActions.append(action);
    }
}

void RecentFiles::addFile(const QString& filePath)
{
    if (m_recentFilesCount == m_maxRecentFiles)
    {
        m_recentFiles.removeLast();
    }
    m_recentFiles[m_recentFilesCount] = filePath;
    ++m_recentFilesCount;
    updateActions();
}

void RecentFiles::updateActions()
{
    for (int i = 0; i < m_maxRecentFiles; ++i)
    {
        const QString &filePath = m_recentFiles[i];
        QAction *action = m_recentFilesActions[i];
        if (filePath.isEmpty())
        {
            action->setText("");
            action->setVisible(false);
        }
        else
        {
            action->setText(filePath);
            action->setVisible(true);
        }
    }
}

QList<QAction *> RecentFiles::actions()
{
    return m_recentFilesActions;
}

int RecentFiles::count() const
{
    return m_recentFilesCount;
}

#pragma once

#include <QList>
#include <QObject>
#include <QString>

class QAction;

/// Manages recent files actions list
class RecentFiles : public QObject {
    Q_OBJECT

public:
    explicit RecentFiles(int maxRecentFiles, QObject* parent = nullptr);

    /// Adds a recent file path
    void addFile(const QString& filePath);
    /// Returns collection of pointers to actions
    QList<QAction*> actions();
    /// Returns number of stored recent files
    int count() const;
    /// Removes all recent files from internal storage
    void clear();

signals:
    /// Triggered when recent file action is clicked. Sends file path stored in clicked action instance
    void filePathSelected(const QString& filePath);

private:
    void updateActions();

    int m_maxRecentFiles;
    int m_recentFilesCount;
    QList<QString> m_recentFiles;
    QList<QAction*> m_recentFilesActions;
};

#pragma once

#include <QList>
#include <QObject>
#include <QString>
// #include <QList>

class QAction;

class RecentFiles : public QObject
{
    Q_OBJECT
    
public:
    explicit RecentFiles(int maxRecentFiles, QObject *parent = nullptr);
//     ~RecentFiles();

    void addFile(const QString &filePath);
    QList<QAction*> actions();
    int count() const;

signals:
    void filePathSelected(const QString &filePath);

private:
    void updateActions();

    int m_maxRecentFiles;
    int m_recentFilesCount;
    QList<QString> m_recentFiles;
    QList<QAction*> m_recentFilesActions;
};

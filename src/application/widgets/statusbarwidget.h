#pragma once

#include <QWidget>

class QHBoxLayout;
class QLabel;
class QPushButton;

class StatusBarWidget : public QWidget {
    Q_OBJECT
public:
    explicit StatusBarWidget(QWidget* parent = nullptr);
    void initButtons();
    void setStatusMessage(const QString& message);
    void setActiveThreadsStatus(int threadsCount);
    void setJobRuntimeStatus(const QString& jobRuntime);
    void setJobStatsStatus(const QString& jobStats);

Q_SIGNALS:
    void toggleToolsWidget(bool visible);
    void toggleLogWidget(bool visible);

private:
    QHBoxLayout* m_mainLayout;
    QPushButton* m_toolsPushButton;
    QPushButton* m_logPushButton;
    QLabel* m_statusMessageLabel;
    QLabel* m_jobStatsLabel;
    QLabel* m_jobRuntimeLabel;
    QLabel* m_activeThreadsLabel;
};

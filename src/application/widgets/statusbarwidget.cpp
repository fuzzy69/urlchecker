#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>

#include "../icons.h"
#include "../texts.h"
#include "statusbarwidget.h"

StatusBarWidget::StatusBarWidget(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("statusBar");
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 2);
    m_mainLayout->setSpacing(2);
    m_toolsPushButton = new QPushButton(QIcon(ICON_HAMMER), QStringLiteral(" Tools"));
    m_toolsPushButton->setObjectName("toolsPushButton");
    m_toolsPushButton->setCheckable(true);
    m_toolsPushButton->setEnabled(false);
    m_logPushButton = new QPushButton(QIcon(ICON_DOCUMENT_LIST), QStringLiteral(" Log"));
    m_logPushButton->setObjectName("logPushButton");
    m_logPushButton->setCheckable(true);
    m_logPushButton->setEnabled(false);
    m_statusMessageLabel = new QLabel(tr("Ready."));
    m_jobRuntimeLabel = new QLabel(" Job runtime: ");
    m_jobRuntimeLabel->setObjectName("jobRuntimeLabel");
    m_jobRuntimeLabel->setStyleSheet(QStringLiteral("border-left: 1px solid #BFBFBF;"));
    m_jobStatsLabel = new QLabel(" Completed 0/0 of 0 items. Success ratio 0.0% ");
    m_jobStatsLabel->setObjectName("jobStatsLabel");
    m_jobStatsLabel->setStyleSheet(QStringLiteral("border-left: 1px solid #BFBFBF;"));
    m_activeThreadsLabel = new QLabel(tr(TEXT_ACTIVE_THREADS));
    m_activeThreadsLabel->setObjectName("activeThreadsLabel");

    m_mainLayout->addWidget(m_toolsPushButton);
    m_mainLayout->addWidget(m_logPushButton);
    m_mainLayout->addWidget(m_statusMessageLabel);
    m_mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored));
    m_mainLayout->addWidget(m_jobRuntimeLabel);
    m_mainLayout->addWidget(m_jobStatsLabel);
    m_mainLayout->addWidget(m_activeThreadsLabel);

    connect(m_toolsPushButton, &QPushButton::clicked, [this](bool checked) {
        Q_EMIT toggleToolsWidget(checked);
    });
    connect(m_logPushButton, &QPushButton::clicked, [this](bool checked) {
        Q_EMIT toggleLogWidget(checked);
    });
}

void StatusBarWidget::initButtons()
{
    m_toolsPushButton->setEnabled(true);
    m_toolsPushButton->setChecked(true);
    m_logPushButton->setEnabled(true);
    m_logPushButton->setChecked(true);
}

void StatusBarWidget::setStatusMessage(const QString& message)
{
    m_statusMessageLabel->setText(message);
}

void StatusBarWidget::setActiveThreadsStatus(int threadsCount)
{
    m_activeThreadsLabel->setText(QString(" Active threads: %1").arg(threadsCount));
    //    m_activeThreadsLabel->setText(tr(TEXT_ACTIVE_THREADS).replace("/", QString(threadsCount)));
}

void StatusBarWidget::setJobRuntimeStatus(const QString& jobRuntime)
{
    m_jobRuntimeLabel->setText(QString(" Job runtime: %1 ").arg(jobRuntime));
}

void StatusBarWidget::setJobStatsStatus(const QString& jobStats)
{
    m_jobStatsLabel->setText(jobStats);
}

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>

#include "../core/applicationbridge.h"
#include "../icons.h"
#include "../texts.h"
#include "logwidget.h"
#include "statusbarwidget.h"
#include "workspacewidget.h"

StatusBarWidget::StatusBarWidget(QWidget* parent)
    : QWidget(parent)
{
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 2);
    m_mainLayout->setSpacing(2);
    m_toolsPushButton = new QPushButton(QIcon(ICON_HAMMER), QStringLiteral(" Tools"));
    m_toolsPushButton->setCheckable(true);
    m_toolsPushButton->setEnabled(false);
    m_logPushButton = new QPushButton(QIcon(ICON_DOCUMENT_LIST), QStringLiteral(" Log"));
    m_logPushButton->setCheckable(true);
    m_logPushButton->setEnabled(false);
    m_statusMessageLabel = new QLabel(tr("Ready."));
    m_jobRuntimeLabel = new QLabel(" Job runtime: ");
    m_jobRuntimeLabel->setStyleSheet(QStringLiteral("border-left: 1px solid #BFBFBF;"));
    m_jobStatsLabel = new QLabel(" Completed 0/0 of 0 items. Success ratio 0.0% ");
    m_jobStatsLabel->setStyleSheet(QStringLiteral("border-left: 1px solid #BFBFBF;"));
    m_activeThreadsLabel = new QLabel(tr(TEXT_ACTIVE_THREADS));

    m_mainLayout->addWidget(m_toolsPushButton);
    m_mainLayout->addWidget(m_logPushButton);
    m_mainLayout->addWidget(m_statusMessageLabel);
    m_mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored));
    m_mainLayout->addWidget(m_jobRuntimeLabel);
    m_mainLayout->addWidget(m_jobStatsLabel);
    m_mainLayout->addWidget(m_activeThreadsLabel);

    connect(m_toolsPushButton, &QPushButton::clicked, [this]() {
        WorkspaceWidget* workspaceWidget = ApplicationBridge::instance().workspaceWidget();
        workspaceWidget->toggleSideTabWidget();
        m_toolsPushButton->setChecked(workspaceWidget->sideTabWidget()->isVisible());
    });
    connect(m_logPushButton, &QPushButton::clicked, [this]() {
        LogWidget* logWidget = ApplicationBridge::instance().logWidget();
        if (logWidget->isVisible()) {
            logWidget->hide();
            m_logPushButton->setChecked(false);
        } else {
            logWidget->show();
            m_logPushButton->setChecked(true);
        }
    });
}

void StatusBarWidget::initButtons()
{
    m_toolsPushButton->setEnabled(true);
    m_toolsPushButton->setChecked(ApplicationBridge::instance().workspaceWidget()->sideTabWidget()->isVisible());
    m_logPushButton->setEnabled(true);
    m_logPushButton->setChecked(ApplicationBridge::instance().logWidget()->isVisible());
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

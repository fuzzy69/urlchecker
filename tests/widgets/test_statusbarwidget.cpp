#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "application/icons.h"
#include "application/texts.h"
#include "application/widgets/statusbarwidget.h"
#include "test_statusbarwidget.h"

void TestStatusBarWidget::testButtons()
{
    StatusBarWidget statusBarWidget;
    statusBarWidget.initButtons();
    auto* logPushButton = statusBarWidget.findChild<QPushButton*>("logPushButton");
    QVERIFY(logPushButton);
    auto* toolsPushButton = statusBarWidget.findChild<QPushButton*>("toolsPushButton");
    QVERIFY(toolsPushButton);
    QSignalSpy spyToggleLogWidget(&statusBarWidget, &StatusBarWidget::toggleLogWidget);
    QSignalSpy spyToggleToolsWidget(&statusBarWidget, &StatusBarWidget::toggleToolsWidget);
    QTest::mouseClick(logPushButton, Qt::LeftButton);
    QTest::mouseClick(toolsPushButton, Qt::LeftButton);
    QCOMPARE(spyToggleLogWidget.count(), 1);
    QCOMPARE(spyToggleToolsWidget.count(), 1);
}

void TestStatusBarWidget::testActiveThreadsStatus()
{
    StatusBarWidget statusBarWidget;
    statusBarWidget.setActiveThreadsStatus(4);
    auto* activeThreadsLabel = statusBarWidget.findChild<QLabel*>("activeThreadsLabel");
    QVERIFY(activeThreadsLabel);
    QCOMPARE(activeThreadsLabel->text(), QString(" Active threads: %1").arg(4));
}

void TestStatusBarWidget::testJobRuntimeStatus()
{
    QString jobRuntime("00:00:00");
    StatusBarWidget statusBarWidget;
    statusBarWidget.setJobRuntimeStatus(jobRuntime);
    auto* jobRuntimeLabel = statusBarWidget.findChild<QLabel*>("jobRuntimeLabel");
    QVERIFY(jobRuntimeLabel);
    QCOMPARE(jobRuntimeLabel->text(), QString(" Job runtime: %1 ").arg(jobRuntime));
}

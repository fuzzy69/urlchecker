#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QIcon>

#include "application/icons.h"
#include "application/texts.h"
#include "application/widgets/sidebarwidget.h"
#include "test_sidebarwidget.h"

void TestSideBarWidget::testAddActions()
{
    SideBarWidget sideBarWidget;
    QCOMPARE(sideBarWidget.actions().count(), 0);
    QAction* action(nullptr);
    action = new QAction(QIcon(ICON_DESKTOP), QStringLiteral(TEXT_WORKSPACE));
    sideBarWidget.addAction(action);
    action = new QAction(QIcon(ICON_GEAR), QStringLiteral(TEXT_SETTINGS));
    sideBarWidget.addAction(action);
    action = new QAction(QIcon(ICON_USER_SILHOUETTE_QUESTION), QStringLiteral(TEXT_USER_AGENTS));
    sideBarWidget.addAction(action);
    action = new QAction(QIcon(ICON_MASK), QStringLiteral(TEXT_PROXIES));
    sideBarWidget.addAction(action);
    action = new QAction(QIcon(ICON_QUESTION), QStringLiteral(TEXT_HELP));
    sideBarWidget.addAction(action);
    QCOMPARE(sideBarWidget.actions().count(), 5);
}

void TestSideBarWidget::testSelectingActions()
{
    SideBarWidget sideBarWidget;
    QAction* workspaceAction = new QAction(QIcon(ICON_DESKTOP), QStringLiteral(TEXT_WORKSPACE));
    sideBarWidget.addAction(workspaceAction);
    QAction* settingsAction = new QAction(QIcon(ICON_GEAR), QStringLiteral(TEXT_SETTINGS));
    sideBarWidget.addAction(settingsAction);
    QCOMPARE(sideBarWidget.actions().count(), 2);
    sideBarWidget.setCurrentAction(0);
    QCOMPARE(sideBarWidget.checkedAction(), workspaceAction);
    sideBarWidget.setCurrentAction(2);
    QCOMPARE(sideBarWidget.checkedAction(), workspaceAction);
    sideBarWidget.setCurrentAction(1);
    QCOMPARE(sideBarWidget.checkedAction(), settingsAction);
}

void TestSideBarWidget::testSignals()
{
    SideBarWidget sideBarWidget;
    QAction* workspaceAction = new QAction(QIcon(ICON_DESKTOP), QStringLiteral(TEXT_WORKSPACE));
    sideBarWidget.addAction(workspaceAction);
    QAction* settingsAction = new QAction(QIcon(ICON_GEAR), QStringLiteral(TEXT_SETTINGS));
    sideBarWidget.addAction(settingsAction);
    QSignalSpy signalSpy(&sideBarWidget, &SideBarWidget::currentActionIndexChanged);
    sideBarWidget.setCurrentAction(2);
    QCOMPARE(signalSpy.count(), 0);
    sideBarWidget.setCurrentAction(0);
    QCOMPARE(signalSpy.count(), 1);
    sideBarWidget.setCurrentAction(0);
    QCOMPARE(signalSpy.count(), 2);
}

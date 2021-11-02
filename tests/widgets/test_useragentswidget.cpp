#include <QAction>
#include <QClipboard>
#include <QDebug>

#include "application/widgets/useragentswidget.h"
#include "test_useragentswidget.h"

void TestUserAgentsWidget::testAddUserAgents()
{
    QString userAgentsText(R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36
Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:66.0) Gecko/20100101 Firefox/66.0
Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36
Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36)");
    UserAgentsWidget userAgentsWidget;
    QApplication::clipboard()->setText(userAgentsText);
    userAgentsWidget.m_pasteUserAgentsAction->trigger();
    QCOMPARE(userAgentsWidget.toPlainText(), userAgentsText);
}

void TestUserAgentsWidget::testRemoveAllUserAgents()
{
    QString userAgentsText(R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36)");
    UserAgentsWidget userAgentsWidget;
    QApplication::clipboard()->setText(userAgentsText);
    QCOMPARE(userAgentsWidget.toPlainText().size(), 0);
    userAgentsWidget.m_pasteUserAgentsAction->trigger();
    QCOMPARE(userAgentsWidget.toPlainText().size(), userAgentsText.size());
    userAgentsWidget.m_removeAllUserAgentsAction->trigger();
    QCOMPARE(userAgentsWidget.toPlainText().size(), 0);
}

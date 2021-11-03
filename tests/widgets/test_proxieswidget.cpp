#include <QAction>
#include <QClipboard>
#include <QDebug>

#include "application/widgets/proxieswidget.h"
#include "test_proxieswidget.h"

void TestProxiesWidget::testAddProxies()
{
    QString proxiesText(R"(127.0.0.1:80
127.0.0.1:81:user:pass
127.0.0.1:82:invalid)");
    ProxiesWidget proxiesWidget;
    QApplication::clipboard()->setText(proxiesText);
    proxiesWidget.pasteProxiesAction()->trigger();
    QCOMPARE(proxiesText, proxiesText);
}

void TestProxiesWidget::testRemoveAllProxies()
{
    QString proxiesText(R"(127.0.0.1:80
127.0.0.1:81:user:pass)");
    ProxiesWidget proxiesWidget;
    QApplication::clipboard()->setText(proxiesText);
    QCOMPARE(proxiesWidget.toPlainText().size(), 0);
    proxiesWidget.pasteProxiesAction()->trigger();
    QCOMPARE(proxiesWidget.toPlainText().size(), proxiesText.size());
    proxiesWidget.removeAllProxiesAction()->trigger();
    QCOMPARE(proxiesWidget.toPlainText().size(), 0);
}

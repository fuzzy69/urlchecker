#include "test_logwidget.h"

#include "application/widgets/logwidget.h"

void TestLogWidget::testOnLog()
{
    LogWidget logWidget;
    logWidget.onLog(QStringLiteral("Hello World!"));
    QCOMPARE(logWidget.text(), QStringLiteral("Hello World!"));
}

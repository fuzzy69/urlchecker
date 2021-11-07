#include <QtTest>
#include <iostream>

#include "test_logwidget.h"
#include "test_proxieswidget.h"
#include "test_sidebarwidget.h"
#include "test_statusbarwidget.h"
#include "test_toolswidget.h"
#include "test_useragentswidget.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    QTEST_SET_MAIN_SOURCE_PATH
    int status = 0;
    auto ASSERT_TEST = [&status, argc, argv](QObject* obj) {
        if (status == 0) {
            status |= QTest::qExec(obj, argc, argv);
        }
        delete obj;
    };

    ASSERT_TEST(new TestLogWidget());
    ASSERT_TEST(new TestProxiesWidget());
    ASSERT_TEST(new TestSideBarWidget());
    ASSERT_TEST(new TestStatusBarWidget());
    ASSERT_TEST(new TestToolsWidget());
    ASSERT_TEST(new TestUserAgentsWidget());

    return status;
}

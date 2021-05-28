#include <QApplication>
#include <QWidget>
#include <QDebug>

#include "cpr/cpr.h"

#include "application/config.h"
#include "application/icons.h"
#include "application/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setWindowIcon(QIcon(ICON_APPLICATION_LOGO));
    application.setApplicationName( APPLICATION_TITLE );
    application.setStyle(QStringLiteral("fusion"));
//     application.setStyleSheet(R"(
// QStatusBar {
//     padding: 5px;
// }
// 
// QStatusBar::item {
//     padding: 5px;
//     border-left: 1px solid #BFBFBF;
// }
//     
// QStatusBar#m_activeThreadsLabel {
//     font-style: italic;
// }
//     )");
    qDebug() << CPR_LIBCURL_VERSION_NUM;
    qDebug() << "OK";
    MainWindow mainWindow;
    mainWindow.show();

    return application.exec();
}

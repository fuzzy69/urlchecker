#include <QApplication>
#include <QIcon>

#include "application/config.h"
#include "application/icons.h"
#include "application/core/mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setWindowIcon(QIcon(ICON_APPLICATION_LOGO));
    application.setApplicationName(APP_TITLE);
    application.setStyle(QStringLiteral("fusion"));
    application.setStyleSheet(R"(
QStatusBar {
    padding: 5px;
}

QStatusBar::item {
    padding: 5px;
    border-left: 1px solid #BFBFBF;
}
    
QStatusBar#m_activeThreadsLabel {
    font-style: italic;
}
    )");
    MainWindow mainWindow;
    mainWindow.show();

    return application.exec();
}

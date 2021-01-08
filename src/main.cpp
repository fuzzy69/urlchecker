#include <QApplication>

#include "application/config.h"
#include "application/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName(APP_TITLE);
    application.setStyle(QStringLiteral("fusion"));
    application.setStyleSheet(R"("
QStatusBar {
    padding: 5px;
}

QStatusBar::item {
    padding: 5px;
    border-left: 1px solid #BFBFBF;
}

QStatusBar#activeToolLabel {
    font-style: italic;
}
"")");

    MainWindow mainWindow;
    mainWindow.show();

    return application.exec();
}


#include <QApplication>

#include "application/config.h"
#include "application/icons.h"
#include "application/mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    application.setWindowIcon(QIcon(ICON_APPLICATION_LOGO));
    application.setApplicationName(APPLICATION_TITLE);
    application.setStyle(QStringLiteral("fusion"));
    MainWindow mainWindow;
    mainWindow.show();

    return application.exec();
}

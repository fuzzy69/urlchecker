#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>

#include "basemainwindow.h"


BaseMainWindow::BaseMainWindow(QWidget* parent) : QMainWindow(parent)
{
}

BaseMainWindow::~BaseMainWindow()
{
}

void BaseMainWindow::centerWindow()
{
    QRect fg = this->frameGeometry();
    QPoint screenCenter = qApp->desktop()->availableGeometry().center();
    fg.moveCenter(screenCenter);
    this->move(fg.topLeft());
}

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>

#include "mainwindowbase.h"

MainWindowBase::MainWindowBase(QWidget* parent)
    : QMainWindow(parent)
{
}

//MainWindowBase::~MainWindowBase()
//{
//}

void MainWindowBase::centerWindow()
{
    QRect fg = this->frameGeometry();
    QPoint screenCenter = qApp->desktop()->availableGeometry().center();
    fg.moveCenter(screenCenter);
    this->move(fg.topLeft());
}

#pragma once

#include <QMainWindow>


class BaseMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaseMainWindow(QWidget *parent = nullptr);
    virtual ~BaseMainWindow();

protected:
    void centerWindow();
    virtual void loadSettings() = 0;
    virtual void saveSettings() = 0;
};

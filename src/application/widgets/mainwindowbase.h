#pragma once

#include <QMainWindow>

class MainWindowBase : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindowBase(QWidget* parent = nullptr);
    //    ~MainWindowBase();

protected:
    void centerWindow();
    virtual void loadSettings() = 0;
    virtual void saveSettings() = 0;
};

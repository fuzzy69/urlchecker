#pragma once

#include <QWidget>

class Ui_ConnectionSettings;

class ConnectionSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionSettingsPage(QWidget *parent = nullptr);
    ~ConnectionSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    Ui_ConnectionSettings* m_ui;

};

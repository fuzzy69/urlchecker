#pragma once

#include <QWidget>

class Ui_ProxiesSettings;

class ProxiesSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProxiesSettingsPage(QWidget *parent = nullptr);
    ~ProxiesSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    Ui_ProxiesSettings* m_ui;
};

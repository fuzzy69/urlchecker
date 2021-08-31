#pragma once

#include <QWidget>

class Ui_GeneralSettings;

class GeneralSettingsPage : public QWidget {
    Q_OBJECT
public:
    explicit GeneralSettingsPage(QWidget* parent = nullptr);
    ~GeneralSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    Ui_GeneralSettings* m_ui;
};

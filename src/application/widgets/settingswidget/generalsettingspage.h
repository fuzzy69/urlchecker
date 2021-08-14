#pragma once

#include <QWidget>

//class QHBoxLayout;
//class QGridLayout;

//namespace Ui { class GeneralSettingsPage; }
class Ui_GeneralSettings;

class GeneralSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralSettingsPage(QWidget *parent = nullptr);
    ~GeneralSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
//    QVBoxLayout *m_mainLayout;
//    Ui::GeneralSettingsPage* ui;
    Ui_GeneralSettings* m_ui;
};

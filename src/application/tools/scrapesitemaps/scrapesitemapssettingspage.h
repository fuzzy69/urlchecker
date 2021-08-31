#pragma once

#include <QWidget>

#include "common.h"

class Ui_ScrapeSitemapsSettings;

class ScrapeSitemapsSettingsPage : public QWidget {
    Q_OBJECT
public:
    explicit ScrapeSitemapsSettingsPage(QWidget* parent = nullptr);
    ~ScrapeSitemapsSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    Ui_ScrapeSitemapsSettings* m_ui;
};

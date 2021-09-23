#pragma once

#include <QWidget>

#include "common.h"

class Ui_settingsPageWidget;

class ScrapeHtmlSettingsPage : public QWidget {
    Q_OBJECT
public:
    explicit ScrapeHtmlSettingsPage(QWidget* parent = nullptr);
    ~ScrapeHtmlSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void setScrapeHtmlStrategy(ScrapeHtmlStrategy strategy);
    ScrapeHtmlStrategy scrapeHtmlStrategy();

    Ui_settingsPageWidget* m_ui;
};

#pragma once

#include <QWidget>

#include "common.h"

class Ui_ScrapeLinksSettings;

class ScrapeLinksSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ScrapeLinksSettingsPage(QWidget *parent = nullptr);
    ~ScrapeLinksSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void setScrapeLinksStrategy(ScrapeLinksStrategy scrapeLinksStrategy);
    ScrapeLinksStrategy scrapeLinksStrategy();

    Ui_ScrapeLinksSettings* m_ui;
};

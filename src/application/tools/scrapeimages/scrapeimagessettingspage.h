#pragma once

#include <QWidget>

#include "common.h"

class Ui_ScrapeImagesSettings;

class ScrapeImagesSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ScrapeImagesSettingsPage(QWidget *parent = nullptr);
    ~ScrapeImagesSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
//    void setScrapeLinksStrategy(ScrapeLinksStrategy scrapeLinksStrategy);
//    ScrapeLinksStrategy scrapeLinksStrategy();

    Ui_ScrapeImagesSettings* m_ui;
};

#pragma once

#include <QMap>
#include <QMetaType>

#include "../config.h"

enum class ToolCategory
{
    MISC,
    SEO,
    EXTRACTORS,
    SCRAPERS,
    URL,
    NONE
};

static QMap<ToolCategory, QString> ToolCategoryTexts{
    {ToolCategory::MISC, QStringLiteral("Misc")},
    {ToolCategory::SEO, QStringLiteral("SEO")},
    {ToolCategory::EXTRACTORS, QStringLiteral("Extractors")},
    {ToolCategory::SCRAPERS, QStringLiteral("Scrapers")},
    {ToolCategory::URL, QStringLiteral("URL")},
    {ToolCategory::NONE, QStringLiteral("")},
};

enum Tools
{
    CHECK_URL_STATUS,
    CHECK_ALEXA_RANK,
    CHECK_SEARCH_ENGINE_INDEX,
    SCRAPE_PROXIES,
    SCRAPE_LINKS,
    SCRAPE_SITEMAPS,
    SCRAPE_EMAILS,
    SCRAPE_PHONE_NUMBERS,
    SCRAPE_IMAGES,
    TEST,
#if APP_DEBUG
    DUMMY,
#endif
    NONE,
};

Q_DECLARE_METATYPE(Tools)

static QMap<Tools, QString> ToolsTexts{
    {Tools::CHECK_URL_STATUS, QStringLiteral("Check URL Status")},
    {Tools::CHECK_ALEXA_RANK, QStringLiteral("Check Alexa Rank")},
    {Tools::CHECK_SEARCH_ENGINE_INDEX, QStringLiteral("Check Search Engine Index")},
    {Tools::SCRAPE_PROXIES, QStringLiteral("Scrape Proxies")},
    {Tools::SCRAPE_LINKS, QStringLiteral("Scrape Links")},
    {Tools::SCRAPE_SITEMAPS, QStringLiteral("Scrape Sitemaps")},
    {Tools::SCRAPE_EMAILS, QStringLiteral("Scrape Emails")},
    {Tools::SCRAPE_PHONE_NUMBERS, QStringLiteral("Scrape Phone Numbers")},
    {Tools::TEST, QStringLiteral("Test")},
#if APP_DEBUG
    {Tools::SCRAPE_EMAILS, QStringLiteral("Scrape Emails")},
#endif
    {Tools::NONE, QStringLiteral("")},
};

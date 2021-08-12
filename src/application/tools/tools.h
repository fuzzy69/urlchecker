#pragma once

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

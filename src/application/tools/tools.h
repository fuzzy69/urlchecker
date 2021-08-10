﻿#pragma once

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
    SCRAPE_PROXIES,
    SCRAPE_LINKS,
    SCRAPE_SITEMAPS,
    TEST,
#if APP_DEBUG
    DUMMY,
#endif
    NONE,
};

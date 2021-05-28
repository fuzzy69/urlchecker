#pragma once

#include "../config.h"


enum Tools
{
    CHECK_URL_STATUS,
    CHECK_ALEXA_RANK,
    SCRAPE_PROXIES,
    TEST,
#if APP_DEBUG
    DUMMY,
#endif
    NONE,
};

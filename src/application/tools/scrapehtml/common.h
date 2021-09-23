#pragma once

#define HTML_DIRECTORY "htmlDirectory"
#define DOWNLOAD_HTML "downloadHtml"
#define SCRAPE_HTML_STRATEGY "scrapeHtmlStrategy"

enum class ScrapeHtmlStrategy {
    FULL_HTML,
    VISIBLE_TEXT
};

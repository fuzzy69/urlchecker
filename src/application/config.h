#pragma once

#define APP_DEBUG true
#define APPLICATION_TITLE "URL Checker"
#define APPLICATION_DESCRIPTION "C++ Qt5 multi-threaded application for checking HTTP response status of urls"
#define USER_AGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:70.0) Gecko/20100101 Firefox/70.0"

// Defaults
#define THREADS 1
#define HTTP_REQUEST_TIMEOUT_ 15
#define USE_PROXIES false
#define MAX_RECENT_FILES 5

#define SETTINGS_FILE "settings.json"
#define PROXIES_FILE "data/proxies.json"
#define USER_AGENTS_FILE "data/user_agents.json"

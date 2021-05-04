#include "proxytools.h"

#include <regex>


std::vector<std::string> extract_proxies(const std::string& text)
{
    std::vector<std::string> proxies;
    static const std::regex regex(R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\s*:?\s*(\d{2,5}))");
    std::sregex_iterator iter(text.begin(), text.end(), regex);
    std::sregex_iterator end;
    while (iter != end)
    {
        proxies.push_back((*iter).str(1) + ":" + (*iter).str(2));
        ++iter;
    }

    return proxies;
}

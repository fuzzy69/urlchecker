#include "utilities.h"

#include <regex>

std::string extract_sitemap_url(const std::string& text)
{
    static const std::regex regex(R"((\b[\w.:/]*[/]+sitemap[\w.]*\b))");
    std::sregex_iterator iter(text.begin(), text.end(), regex);
    std::sregex_iterator end;
    while (iter != end)
    {
        return iter->str(1);
    }

    return {};
}

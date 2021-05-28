#include <regex>

std::optional<int> extract_alexa_rank(const std::string& html)
{
    static std::regex regex("RANK=\"(\\d+)\"");
    std::smatch match;
    if (std::regex_search(html, match, regex))
        return std::stoi(match[1]);
    else
        return {};
}

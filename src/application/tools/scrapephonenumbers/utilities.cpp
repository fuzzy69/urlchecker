#include "utilities.h"

#include <regex>
#include <unordered_set>

std::unordered_set<std::string> extract_phone_numbers(const std::string &text)
{
    std::unordered_set<std::string> phone_numbers;
    static const std::regex regex(R"([\+]?[(]?[0-9]{3}[)]?[-\s\.]?[0-9]{3}[-\s\.]?[0-9]{4,6})");
    std::sregex_iterator iter(text.begin(), text.end(), regex);
    std::sregex_iterator end;
    while (iter != end)
    {
        if (phone_numbers.count(iter->str(0)) == 0)
            phone_numbers.insert(iter->str(0));
        ++iter;
    }

    return phone_numbers;
}

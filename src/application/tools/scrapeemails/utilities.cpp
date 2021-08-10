#include "utilities.h"

#include <regex>
#include <unordered_set>

std::unordered_set<std::string> extract_emails(const std::string &text)
{
    std::unordered_set<std::string> emails;
    static const std::regex regex(R"(\w+(\.\w+)*@\w+(\.\w+)*(\.\w+([-.]\w+)*))");
    std::sregex_iterator iter(text.begin(), text.end(), regex);
    std::sregex_iterator end;
    while (iter != end)
    {
        if (emails.count(iter->str(0)) == 0)
            emails.insert(iter->str(0));
        ++iter;
    }

    return emails;
}

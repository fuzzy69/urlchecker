#include "my/text.h"

#include <sstream>
#include <string>


namespace my
{
namespace text
{
std::vector<std::string> split(const std::string& text, const std::string& delimiter)
{
    std::vector<std::string> chunks;
    size_t pos_start = 0, pos_end, delimiter_length = delimiter.length();
    std::string chunk;
    while ((pos_end = text.find(delimiter, pos_start)) != std::string::npos)
    {
        chunk = text.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delimiter_length;
        chunks.push_back(chunk);
    }
    chunks.push_back(text.substr(pos_start));

    return chunks;
}

std::vector<std::string> split_lines(const std::string& text)
{
    std::vector<std::string> lines;
    std::stringstream stream(text);
    for (std::string line; getline(stream, line, '\n');)
        lines.push_back(line);

    return lines;
}

std::string left_strip(const std::string& text)
{
    std::size_t pos = text.find_first_not_of(" \t\n\r\f\v");
    if (pos == std::string::npos)
        return text;

    return text.substr(pos, text.length() - pos);
}

std::string right_strip(const std::string& text)
{
    std::size_t pos = text.find_last_not_of(" \t\n\r\f\v");
    if (pos == std::string::npos)
        return text;

    return text.substr(0, pos + 1);
}

std::string strip(const std::string& text)
{
    return right_strip(left_strip(text));
}

bool starts_with(const std::string& text, const std::string& prefix)
{
    return text.compare(0, prefix.length(), prefix) == 0;
}
}
}

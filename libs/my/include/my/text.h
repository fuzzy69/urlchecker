#pragma once

#include <iostream>
#include <string>
#include <vector>


namespace my
{
namespace text
{
std::vector<std::string> split(const std::string& text, const std::string& delimiter);

std::vector<std::string> split_lines(const std::string& text);

std::string left_strip(const std::string& text);

std::string right_strip(const std::string& text);

std::string strip(const std::string& text);

bool starts_with(const std::string& text, const std::string& prefix);
}
}

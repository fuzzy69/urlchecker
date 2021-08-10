#pragma once

#include <string>
#include <unordered_set>

std::unordered_set<std::string> extract_emails(const std::string& text);

#pragma once

#include <string>

#include "tidy.h"

class TidyHtml
{
public:
    TidyHtml();
    ~TidyHtml();
    ///
    std::string process(const std::string& html);

private:
    TidyDoc m_doc;
};

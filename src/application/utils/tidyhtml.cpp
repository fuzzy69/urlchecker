#include "tidybuffio.h"

#include "tidyhtml.h"

TidyHtml::TidyHtml() : m_doc(tidyCreate())
{
    tidyOptSetBool(m_doc, TidyXhtmlOut, yes)
    and tidyOptSetBool(m_doc, TidyQuiet, yes)
    and tidyOptSetBool(m_doc, TidyShowWarnings, no);
}

TidyHtml::~TidyHtml()
{
    tidyRelease(m_doc);
}

std::string TidyHtml::process(const std::string& html)
{
    std::string result;
    TidyBuffer buffer = {nullptr, nullptr, 0, 0, 0};
    // TODO: Improve this and add error handling
    tidyParseString(m_doc, html.c_str());
    tidyCleanAndRepair(m_doc);
    tidySaveBuffer(m_doc, &buffer);
    uint buffer_size = buffer.size;
    char* buffer_chars = static_cast<char*>(malloc(buffer_size + 10));
    tidySaveString(m_doc, buffer_chars, &buffer_size);
    result = std::string(buffer_chars);
    if (buffer.size)
        tidyBufFree(&buffer);
    else
        result = "";
    
    return result;
}

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "pugixml.hpp"

struct HTMLElement
{
    explicit HTMLElement(const pugi::xpath_node& node);
    ///
    std::string text() const;
    ///
    std::string attribute(const std::string& name);
    
private:
    pugi::xpath_node m_node;
};

class SimpleDOM
{
public:
    explicit SimpleDOM();
    ///
    bool from_string(const std::string& text);
    ///
    std::vector<HTMLElement> select_all(const std::string& query) const;
    ///
    std::optional<HTMLElement> select_first(const std::string& query) const;
    
private:
    pugi::xml_document m_doc;
};

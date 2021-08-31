#include "simpledom.h"

HTMLElement::HTMLElement(const pugi::xpath_node& node)
    : m_node(node)
{
}

std::string HTMLElement::text() const
{
    return std::string(m_node.node().text().as_string());
}

std::string HTMLElement::attribute(const std::string& name)
{
    return std::string(m_node.node().attribute(name.c_str()).value());
}

SimpleDOM::SimpleDOM()
    : m_doc(pugi::xml_document())
{
}

bool SimpleDOM::from_string(const std::string& text)
{
    pugi::xml_parse_result result = m_doc.load_string(text.c_str());

    return result;
}

std::vector<HTMLElement> SimpleDOM::select_all(const std::string& query) const
{
    // TODO: Validate XPath
    std::vector<HTMLElement> elements;
    pugi::xpath_node_set nodes = m_doc.select_nodes(query.c_str());
    for (const auto& node : nodes)
        elements.push_back(HTMLElement(node));

    return elements;
}

std::optional<HTMLElement> SimpleDOM::select_first(const std::string& query) const
{
    // TODO: Validate XPath
    pugi::xpath_node node = m_doc.select_node(query.c_str());

    if (node)
        return HTMLElement(node);
    else
        return {};
}

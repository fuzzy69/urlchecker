#pragma once

#include <memory>
#include <unordered_map>

#include "../tools/tool.h"
#include "../tools/tools.h"

class ToolsManager final {
public:
    static ToolsManager& instance();

    void addTool(std::unique_ptr<Tool> tool);
    Tool& currentTool();
    void setCurrentTool(const QString& toolName);
    Tool* getTool(const QString& toolName);
    Tool* getTool(Tools toolId);
    std::unordered_map<Tools, std::unique_ptr<Tool>>& tools();

private:
    explicit ToolsManager();

#if UNIT_TESTING
public:
#else
private:
#endif
    std::unordered_map<Tools, std::unique_ptr<Tool>> m_toolIdToolMap;
    Tools m_currentTool;
};

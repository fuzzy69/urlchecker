#pragma once

#include <QMap>

#include "../tools/tool.h"
#include "../tools/tools.h"

class ToolsManager final
{
public:
    static ToolsManager &instance();

    void addTool(const QIcon &icon, const QString &text);
    void addTool(const Tool &tool, bool current = false);
    Tool currentTool() const;
    void setCurrentTool(const QString &toolName);
    Tool getTool(const QString &toolName) const;
    QMap<QString, Tool> tools() const;

private:
    /// Disallow default public constructor
    explicit ToolsManager();

    QMap<QString, Tool> m_tools;
    Tool m_currentTool;
};

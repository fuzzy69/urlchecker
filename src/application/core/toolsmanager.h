#pragma once

#include <QMap>

#include "../tools/tool.h"
#include "../tools/tools.h"

class ToolsManager final
{
public:
    static ToolsManager &instance();
    ~ToolsManager();
    void addTool(Tool* tool);
    Tool& currentTool() const;
    void setCurrentTool(const QString &toolName);
    Tool& getTool(const QString &toolName) const;
    Tool& getTool(Tools toolId) const;
    QMap<QString, Tool*> tools() const;

private:
    /// Disallow default public constructor
    explicit ToolsManager();

    QMap<QString, Tool*> m_toolNameToolMap;
    QMap<Tools, Tool*> m_toolIdToolMap;
    Tool* m_currentTool = nullptr;
};

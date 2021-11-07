#include <iostream>
#include <memory>
#include <string>

#include "catch.hpp"

#include "application/core/toolsmanager.h"
#include "application/tools/dummy/dummytool.h"
#include "application/tools/test/testtool.h"
#include "application/tools/tools.h"

TEST_CASE("Test tools manager add tool", "[toolsmanageraddtool]")
{
    REQUIRE(ToolsManager::instance().m_currentTool == Tools::NONE);
    ToolsManager::instance().addTool(std::unique_ptr<Tool>(new DummyTool));
    ToolsManager::instance().addTool(std::unique_ptr<Tool>(new TestTool));
    REQUIRE(ToolsManager::instance().m_toolIdToolMap.size() == 2);
}

TEST_CASE("Test tools manager current tool", "[toolsmanagercurrenttool]")
{
    REQUIRE(ToolsManager::instance().m_currentTool == Tools::NONE);
    ToolsManager::instance().addTool(std::unique_ptr<Tool>(new DummyTool));
    ToolsManager::instance().addTool(std::unique_ptr<Tool>(new TestTool));
    ToolsManager::instance().setCurrentTool(QStringLiteral("Dummy"));
    REQUIRE(ToolsManager::instance().m_currentTool == Tools::DUMMY);
}

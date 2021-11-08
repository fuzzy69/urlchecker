#include <iostream>
#include <string>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QVariant>

#include "catch.hpp"

#include "application/core/settings.h"

TEST_CASE("Test settings class", "[settings]")
{
    char* argv[] = {};
    int argc = 1;
    QApplication application(argc, argv);
    QDir currentDir(QApplication::applicationDirPath());
    QString settingsFile = currentDir.absoluteFilePath("data/settings.json");
    application.exit();
    REQUIRE(Settings::instance().load(settingsFile));
    REQUIRE(Settings::instance().value("parallelTasks").toInt() == 1);
    REQUIRE(!Settings::instance().value("useProxies").toBool());
    REQUIRE(Settings::instance().value("userAgentsFilePath").toString() == QStringLiteral("/mnt/ramdisk/urlchecker/bin/data/user_agents.txt"));
}

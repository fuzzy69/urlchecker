#pragma once

#include <QDir>
#include <QString>

void initSettings(const QDir& applicationDir, const QString& settingsFilePath);

void initProxies(const QString& proxiesFilePath);

void initUserAgents(const QString& proxiesFilePath);

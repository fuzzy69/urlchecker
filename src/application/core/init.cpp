#include "init.h"

#include <QVariant>

#include "../config.h"
#include "../texts.h"
#include "settings.h"

void initSettings(const QDir& applicationDir, const QString& settingsFilePath)
{
    Settings::instance().setFilePath(settingsFilePath);
    Settings::instance().setValue(QStringLiteral(TEXT_THREADS), QVariant(THREADS));
    Settings::instance().setValue(QStringLiteral(TEXT_TIMEOUT), QVariant(HTTP_REQUEST_TIMEOUT_));
    Settings::instance().setValue(QStringLiteral(TEXT_USE_PROXIES), QVariant(USE_PROXIES));
    Settings::instance().setValue(QStringLiteral(TEXT_LAST_DIRECTORY), QVariant(applicationDir.absolutePath()));
    Settings::instance().setValue(QStringLiteral(TEXT_PROXIES_FILE), QVariant(applicationDir.filePath(QStringLiteral(PROXIES_FILE))));
    Settings::instance().setValue(QStringLiteral(TEXT_USER_AGENTS_FILE),
        QVariant(applicationDir.filePath(QStringLiteral(USER_AGENTS_FILE))));
}

void initProxies(const QString& proxiesFilePath)
{
}

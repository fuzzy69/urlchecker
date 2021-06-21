#include "misc.h"

#include <QFile>

#include "my/browserutils.h"
#include "my/file.h"
#include "my/proxymanager.h"
#include "my/proxyutils.h"
#include "my/useragents.h"

#include "../texts.h"

using my::browser::UserAgentsManager;
using my::filesystem::File;
using my::data::USER_AGENTS_TEXT;
using my::network::loadProxiesFromFile;
using my::network::ProxyManager;

void initUserAgents(const QString& userAgentsFilePath)
{
    if (!QFile::exists(userAgentsFilePath))
    {
        File::writeTextFile(userAgentsFilePath, QString(USER_AGENTS_TEXT));
    }
    for (auto& line : File::readTextLines(userAgentsFilePath))
    {
        UserAgentsManager<QString>::instance().add_user_agent(line.trimmed());
    }
}

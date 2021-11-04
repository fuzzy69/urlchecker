#include "misc.h"

#include <QFile>

#include "../../common/file.h"
#include "../../common/useragentmanager.h"
#include "../../data/useragents.h"

#include "../texts.h"

using common::browser::UserAgentManager;
using common::filesystem::File;
using data::USER_AGENTS_TEXT;

void initUserAgents(const QString& userAgentsFilePath)
{
    if (!QFile::exists(userAgentsFilePath)) {
        File::writeTextFile(userAgentsFilePath, QString(USER_AGENTS_TEXT));
    }
    for (auto& line : File::readTextLines(userAgentsFilePath)) {
        UserAgentManager::self().add_user_agent(line.trimmed().toStdString());
    }
}

#include <QFile>
#include <QList>
#include <QString>

#include "file.h"
#include "useragents.h"


UserAgents::UserAgents()
{
    m_userAgents = QList<QString>();
}

UserAgents UserAgents::fromFile(const QString& filePath)
{
    UserAgents userAgents;
    if (QFile::exists(filePath))
    {
        for (auto &line : File::readTextLines(filePath))
        {
            userAgents.addUserAgent(line.trimmed());
        }
    }

    return userAgents;
}

void UserAgents::addUserAgent(const QString& userAgent)
{
    m_userAgents.append(userAgent);
}


QString UserAgents::get() const
{
    return m_userAgents.at(0);
}

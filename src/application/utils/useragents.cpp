#include <QFile>
#include <QList>
#include <QString>
#include <QDebug>

// #include "libs/random/include/effolkronium/random.hpp"
#include "file.h"
#include "useragents.h"
#include "rand.h"


// using Random = effolkronium::random_static;

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
    return m_userAgents.at(Rand::instance().get(0, m_userAgents.length() - 1));
//     return m_userAgents.at(Random::get(0, m_userAgents.length() - 1));
}

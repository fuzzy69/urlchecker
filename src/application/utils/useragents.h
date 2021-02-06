#pragma once

#include <QList>
#include <QString>

/// Manages collection of user agent strings
class UserAgents
{
public:
    UserAgents();
    /// Returns an instance of UserAgents object after loading user agent strings from given text file
    UserAgents static fromFile(const QString &filePath);
    /// Returns random user agent string
    QString get() const;
    /// Ads user agent string to a user agents collection
    void addUserAgent(const QString &userAgent);

private:
    QList<QString> m_userAgents;
};

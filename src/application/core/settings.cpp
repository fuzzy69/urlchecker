#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "settings.h"

Settings::Settings()
    : m_settings(QVariantMap())
    , m_settingsFilePath(QStringLiteral(""))
{
}

Settings::~Settings()
{
    save();
}

Settings& Settings::instance()
{
    static Settings instance;

    return instance;
}

void Settings::setFilePath(const QString& filePath)
{
    m_settingsFilePath = filePath;
}

bool Settings::load(const QString& filePath)
{
    if (!QFile::exists((filePath.isEmpty() ? m_settingsFilePath : filePath)))
        return false;
    QFile file((filePath.isEmpty() ? m_settingsFilePath : filePath));
    file.open(QFile::ReadOnly);
    QByteArray buffer = file.readAll();
    file.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(buffer);
    m_settings = jsonDocument.object().toVariantMap();

    return true;
}

void Settings::save(const QString& filePath)
{
    QJsonObject jsonObject = QJsonObject::fromVariantMap(m_settings);
    QJsonDocument jsonDocument;
    jsonDocument.setObject(jsonObject);
    QFile file;
    file.setFileName((filePath.isEmpty() ? m_settingsFilePath : filePath));
    file.open(QFile::WriteOnly);
    file.write(jsonDocument.toJson());
    file.close();
}

void Settings::setValue(const QString& key, const QVariant& value)
{
    m_settings.insert(key, value);
}

QVariant Settings::value(const QString& key) const
{
    return m_settings[key];
}

void Settings::dump()
{
}

QVariantMap Settings::get() const
{
    return m_settings;
}

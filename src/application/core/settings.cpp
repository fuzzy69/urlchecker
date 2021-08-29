// #include <QApplication>
// #include <QDir>
// #include <QScopedPointer>
// #include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QVariantMap>

#include "settings.h"

Settings::Settings()
{
//     QDir applicationDir(QApplication::applicationDirPath());
//     m_settingsFilePath = applicationDir.absoluteFilePath("settings.json");
    m_settings = QVariantMap();
//     load();
}

Settings::~Settings()
{
    save();
}

Settings & Settings::instance()
{
    static Settings instance;

    return instance;
}

void Settings::setFilePath(const QString& filePath)
{
    m_settingsFilePath = filePath;
}

void Settings::load(const QString& filePath)
{
    if (!QFile::exists((filePath.isEmpty()? m_settingsFilePath : filePath)))
        return;
    QFile file((filePath.isEmpty()? m_settingsFilePath : filePath));
    file.open(QFile::ReadOnly);
    QByteArray buffer = file.readAll();
    file.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(buffer);
    m_settings = jsonDocument.object().toVariantMap();
}

void Settings::save(const QString& filePath)
{
    QJsonObject jsonObject = QJsonObject::fromVariantMap(m_settings);
    QJsonDocument jsonDocument;
    jsonDocument.setObject(jsonObject);
    QFile file;
    file.setFileName((filePath.isEmpty()? m_settingsFilePath : filePath));
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

#pragma once

// #include <QObject>
// #include <QVariantMap>


/// Simple settings class for storing application settings into a JSON file
class Settings final
{
public:
    ~Settings();

    /// Returns Settings class singleton instance
    static Settings &instance();
    /// Returns settings key value
    QVariant value(const QString &key) const;
    /// Sets value of a settings key
    void setValue(const QString &key, const QVariant &value);
    ///
    void setFilePath(const QString &filePath);

    /// Loads settings from a file. If file path is not passed it uses default path
    void load(const QString &filePath = QString());
    /// Saves current settings to a file. If file path is not passed it uses default path
    void save(const QString &filePath = QString());
    /**
    * create a new main window, use given config if any for restore
    * @param sconfig session config object
    * @param sgroup session group for this window
    * @return new constructed main window
    */
    void dump();

private:
    /// Disallow default public constructor
    explicit Settings();

    QVariantMap m_settings;
    QString m_settingsFilePath;
};

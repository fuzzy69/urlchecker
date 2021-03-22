#pragma once

#include <QObject>
#include <QVariant>


/// Simple settings class for storing application settings into a JSON file
class Settings final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Settings)

public:
    ~Settings() override;

    /// Returns Settings class singleton instance
    static Settings &instance();
    /// Returns settings key value
    QVariant value(const QString &key) const;
    /// Sets value of a settings key
    void setValue(const QString &key, const QVariant &value);
    ///
    void setFilePath(const QString &filePath);

public Q_SLOTS:
    /// Loads settings from a file. If file path is not passed it uses default path
    void load(const QString &filePath = QString());
    /// Saves current settings to a file. If file path is not passed it uses default path
    void save(const QString &filePath = QString());

Q_SIGNALS:
    /// Signals when settings key was updated with new value
    void updated();

private:
    /// Disallow default public constructor
    explicit Settings(QObject *parent = nullptr);

    QVariantMap m_settings;
    QString m_settingsFilePath;
};

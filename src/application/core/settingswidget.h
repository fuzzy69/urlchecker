#pragma once

#include <QWidget>

class QVBoxLayout;
class QSpinBox;
class QLabel;
class QGroupBox;
class QGridLayout;
class QCheckBox;


class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

    int threadCount() const;

protected:
    void hideEvent(QHideEvent *event) override;

private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_threadsLabel;
    QLabel *m_timeoutLabel;
    QLabel *m_useProxiesLabel;
    QGroupBox *m_connectionGroupBox;
    QGroupBox *m_proxiesGroupBox;
    QSpinBox *m_threadsSpinBox;
    QSpinBox *m_timeoutSpinBox;
    QGridLayout *m_connectionLayout;
    QGridLayout *m_proxiesLayout;
    QCheckBox *m_useProxiesCheckBox;
    QString m_settingsFilePath;
};

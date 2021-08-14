﻿#pragma once

#include <QWidget>

class QCheckBox;
class QGroupBox;
class QGridLayout;
class QLabel;
class QSpinBox;
class QHBoxLayout;
class QVBoxLayout;
class QListWidget;
class QStackedWidget;

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

protected:
    void hideEvent(QHideEvent *event) override;
    void showEvent ( QShowEvent * event ) override;

private:
    QHBoxLayout *m_mainLayout;
//    QLabel *m_threadsLabel;
//    QLabel *m_timeoutLabel;
//    QLabel *m_verifySslLabel;
//    QLabel *m_useProxiesLabel;
//    QGroupBox *m_connectionGroupBox;
//    QGroupBox *m_proxiesGroupBox;
//    QSpinBox *m_threadsSpinBox;
//    QSpinBox *m_timeoutSpinBox;
//    QCheckBox *m_verifySslCheckBox;
//    QGridLayout *m_connectionLayout;
//    QGridLayout *m_proxiesLayout;
//    QCheckBox *m_useProxiesCheckBox;
//    QString m_settingsFilePath;
    QListWidget* m_categoriesListWidget;
    QStackedWidget* m_contentsStackedWidget;
};

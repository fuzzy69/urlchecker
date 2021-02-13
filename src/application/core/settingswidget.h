#pragma once

#include <QWidget>

class QVBoxLayout;
class QSpinBox;
class QLabel;
class QGroupBox;
class QGridLayout;


class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_threadsLabel;
    QLabel *m_timeoutLabel;
    QGroupBox *m_groupBox1;
    QGroupBox *m_groupBox2;
    QSpinBox *m_threadsSpinBox;
    QSpinBox *m_timeoutSpinBox;
    QGridLayout *m_groupBox1Layout;
};

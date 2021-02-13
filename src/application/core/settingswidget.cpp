#include "settingswidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>


SettingsWidget::SettingsWidget ( QWidget* parent ) :
    QWidget(parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_groupBox1 = new QGroupBox("Group 1");
    m_groupBox2 = new QGroupBox("Group 2");
    m_threadsLabel = new QLabel("Threads");
    m_timeoutLabel = new QLabel("Timeout");
    m_threadsSpinBox = new QSpinBox;
    m_timeoutSpinBox = new QSpinBox;
    m_groupBox1Layout = new QGridLayout(m_groupBox1);
    m_groupBox1Layout->addWidget(m_threadsLabel, 0, 0);
    m_groupBox1Layout->addWidget(m_threadsSpinBox, 0, 1);
    m_groupBox1Layout->addWidget(m_timeoutLabel, 1, 0);
    m_groupBox1Layout->addWidget(m_timeoutSpinBox, 1, 1);
    m_mainLayout->addWidget(m_groupBox1);
    m_mainLayout->addStretch(0);
}

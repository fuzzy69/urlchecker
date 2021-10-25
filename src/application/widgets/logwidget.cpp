#include <QHBoxLayout>
#include <QPlainTextEdit>

#include "logwidget.h"

LogWidget::LogWidget(QWidget* parent)
    : QWidget(parent)
{
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 2);
    m_mainLayout->setSpacing(0);
    m_logPlainTextEdit = new QPlainTextEdit;
    m_logPlainTextEdit->setReadOnly(true);
    m_logPlainTextEdit->setMaximumBlockCount(10000);
    m_mainLayout->addWidget(m_logPlainTextEdit);
}

void LogWidget::onLog(const QString& message)
{
    m_logPlainTextEdit->appendPlainText(message);
}

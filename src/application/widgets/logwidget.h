#pragma once

#include <QWidget>

class QHBoxLayout;
class QPlainTextEdit;

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = nullptr);

signals:

public Q_SLOTS:
    void onLog(const QString& message);

private:
    QHBoxLayout *m_mainLayout;
    QPlainTextEdit *m_logPlainTextEdit;
};

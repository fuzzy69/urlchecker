#pragma once

#include <QTextBrowser>

class QHelpEngine;
class QWidget;

class HelpBrowser : public QTextBrowser {
public:
    HelpBrowser(QHelpEngine* helpEngine, QWidget* parent = nullptr);
    QVariant loadResource(int type, const QUrl& name) override;

private:
    QHelpEngine* m_helpEngine;
};

#pragma once

#include <QPoint>
#include <QTextEdit>

class UserAgentsWidget : public QTextEdit {
    Q_OBJECT

public:
    explicit UserAgentsWidget(QTextEdit* parent = nullptr);

#if UNIT_TESTING
    QAction* pasteUserAgentsAction()
    {
        return m_pasteUserAgentsAction;
    }
    QAction* removeAllUserAgentsAction() { return m_removeAllUserAgentsAction; }
#endif

public slots:
    void showCustomContextMenu(const QPoint& point);

private:
    QAction* m_pasteUserAgentsAction;
    QAction* m_loadDefaultUserAgentsAction;
    QAction* m_removeAllUserAgentsAction;
};

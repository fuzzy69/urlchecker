#pragma once

#include <QPoint>
#include <QTextEdit>

class ProxiesWidget : public QTextEdit {
    Q_OBJECT

public:
    explicit ProxiesWidget(QTextEdit* parent = nullptr);

#if UNIT_TESTING
    QAction* pasteProxiesAction()
    {
        return m_pasteProxiesAction;
    }
    QAction* removeAllProxiesAction() { return m_removeAllProxiesAction; }
#endif

public slots:
    void showCustomContextMenu(const QPoint& point);

private:
    QAction* m_pasteProxiesAction;
    QAction* m_removeAllProxiesAction;
};

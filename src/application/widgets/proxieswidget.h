#pragma once

#include <QPoint>
#include <QTextEdit>

class ProxiesWidget : public QTextEdit {
    Q_OBJECT

public:
    explicit ProxiesWidget(QTextEdit* parent = nullptr);

    // TODO: Enable only for testing
    QAction* pasteProxiesAction() { return m_pasteProxiesAction; }
    QAction* removeAllProxiesAction() { return m_removeAllProxiesAction; }

public slots:
    void showCustomContextMenu(const QPoint& point);

private:
    QAction* m_pasteProxiesAction;
    QAction* m_removeAllProxiesAction;
};

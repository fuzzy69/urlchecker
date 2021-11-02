#pragma once

#include <QPoint>
#include <QTextEdit>

//QT_BEGIN_NAMESPACE
//class QAction;
//QT_END_NAMESPACE

class UserAgentsWidget : public QTextEdit {
    Q_OBJECT

public:
    explicit UserAgentsWidget(QTextEdit* parent = nullptr);

    QAction* m_pasteUserAgentsAction;
    QAction* m_removeAllUserAgentsAction;

public slots:
    void showCustomContextMenu(const QPoint& point);
    //    void pasteUserAgents();

    //private:
    //    QAction* m_pasteUserAgentsAction;
    //    QAction* m_removeAllUserAgentsAction;
};

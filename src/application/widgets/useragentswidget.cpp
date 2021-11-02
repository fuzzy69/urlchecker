#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QDebug>
#include <QIcon>
#include <QMenu>
#include <QPoint>
#include <QRegularExpression>
#include <QTextEdit>

#include "../icons.h"
#include "useragentswidget.h"

UserAgentsWidget::UserAgentsWidget(QTextEdit* parent)
    : QTextEdit(parent)
{
    setReadOnly(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTextEdit::customContextMenuRequested, this, &UserAgentsWidget::showCustomContextMenu);
    setPlaceholderText("Paste user agents here, one user agent per line");
    m_pasteUserAgentsAction = new QAction(QIcon(ICON_CLIPBOARD_PASTE), "Paste User Agents");
    m_removeAllUserAgentsAction = new QAction(QIcon(ICON_CROSS), "Remove All User Agents");
    connect(m_pasteUserAgentsAction, &QAction::triggered, [this] {
        QString clipboardText(QApplication::clipboard()->text().trimmed());
        for (auto& line : clipboardText.split(QRegularExpression("\n|\r\n|\r"), QString::SkipEmptyParts)) {
            line = line.trimmed();
            if (line.size() > 0) {
                append(line);
            }
        }
    });
    connect(m_removeAllUserAgentsAction, &QAction::triggered, this, &QTextEdit::clear);
}

void UserAgentsWidget::showCustomContextMenu(const QPoint& point)
{
    QPoint globalPoint = this->mapToGlobal(point);
    QMenu menu(this);
    menu.addAction(m_pasteUserAgentsAction);
    menu.addAction(m_removeAllUserAgentsAction);
    menu.exec(globalPoint);
}

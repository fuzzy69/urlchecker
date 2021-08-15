#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QDebug>
#include <QIcon>
#include <QPoint>
#include <QRegularExpression>
#include <QMenu>
#include <QTextEdit>

#include "useragentswidget.h"
#include "../icons.h"


UserAgentsWidget::UserAgentsWidget(QTextEdit* parent) : QTextEdit(parent)
{
    setReadOnly(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTextEdit::customContextMenuRequested, this, &UserAgentsWidget::showCustomContextMenu);
    setPlaceholderText("Paste user agents here, one user agent per line");
}

void UserAgentsWidget::showCustomContextMenu(const QPoint& point)
{
    QPoint globalPoint= this->mapToGlobal(point);
    QMenu menu(this);
    QAction *pasteUserAgentsAction = menu.addAction(QIcon(ICON_CLIPBOARD_PASTE), "Paste User Agents");
    QAction *removeAllUserAgentsAction = menu.addAction(QIcon(ICON_CROSS), "Remove All User Agents");
    QAction *action = menu.exec(globalPoint);
    if (action == pasteUserAgentsAction)
    {
        for (auto &line : QApplication::clipboard()->text().trimmed().split(QRegularExpression("\n|\r\n|\r"), QString::SkipEmptyParts))
        {
            QStringList chunks = line.split(":");
            if (chunks.length() == 2 || chunks.length() == 4)
                append(line);
        }
    }
    else if (action == removeAllUserAgentsAction)
    {
        clear();
    }
}

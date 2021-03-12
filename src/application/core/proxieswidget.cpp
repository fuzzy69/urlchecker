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

#include "proxieswidget.h"


ProxiesWidget::ProxiesWidget(QTextEdit* parent) : QTextEdit(parent)
{
    setReadOnly(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTextEdit::customContextMenuRequested, this, &ProxiesWidget::showCustomContextMenu);
    setPlaceholderText("Paste proxies here, one proxy per line in format HOST:PORT:USERNAME:PASSWORD or just HOST:PORT");
}

void ProxiesWidget::showCustomContextMenu(const QPoint& point)
{
    QPoint globalPoint= this->mapToGlobal(point);
    QMenu menu(this);
    QAction *pasteProxiesAction = menu.addAction(QIcon(":assets/icons/clipboard-paste.png"), "Paste Proxies");
    QAction *removeAllProxiesAction = menu.addAction(QIcon(":assets/icons/cross.png"), "Remove All Proxies");
    QAction *action = menu.exec(globalPoint);
    if (action == pasteProxiesAction)
    {
        for (auto &line : QApplication::clipboard()->text().trimmed().split(QRegularExpression("\n|\r\n|\r"), QString::SkipEmptyParts))
        {
            QStringList chunks = line.split(":");
            if (chunks.length() == 2 || chunks.length() == 4)
                append(line);
        }
    }
    else if (action == removeAllProxiesAction)
    {
        clear();
    }
}

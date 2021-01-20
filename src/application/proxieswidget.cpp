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
}

void ProxiesWidget::showCustomContextMenu(const QPoint& point)
{
    QPoint globalPoint= this->mapToGlobal(point);
    QMenu menu(this);
    QAction *pasteProxiesAction = menu.addAction(QIcon(":assets/icons/control.png"), "Paste Proxies");
    QAction *removeAllProxiesAction = menu.addAction(QIcon(":assets/icons/control.png"), "Remove All Proxies");
    QAction *action = menu.exec(globalPoint);
    if (action == pasteProxiesAction)
    {
        for (auto &line : QApplication::clipboard()->text().trimmed().split(QRegularExpression("\n|\r\n|\r"), QString::SkipEmptyParts))
        {
            append(line);
        }
    }
    else if (action == removeAllProxiesAction)
    {
        clear();
    }
}

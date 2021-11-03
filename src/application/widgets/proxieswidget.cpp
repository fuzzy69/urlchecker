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
#include "proxieswidget.h"

ProxiesWidget::ProxiesWidget(QTextEdit* parent)
    : QTextEdit(parent)
{
    setReadOnly(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTextEdit::customContextMenuRequested, this, &ProxiesWidget::showCustomContextMenu);
    setPlaceholderText("Paste proxies here, one proxy per line in format HOST:PORT:USERNAME:PASSWORD or just HOST:PORT");
    m_pasteProxiesAction = new QAction(QIcon(ICON_CLIPBOARD_PASTE), "Paste Proxies");
    m_removeAllProxiesAction = new QAction(QIcon(ICON_CROSS), "Remove All Proxies");
    connect(m_pasteProxiesAction, &QAction::triggered, [this] {
        QString clipboardText(QApplication::clipboard()->text().trimmed());
        for (auto& line : clipboardText.split(QRegularExpression("\n|\r\n|\r"), QString::SkipEmptyParts)) {
            QStringList chunks = line.trimmed().split(":");
            if (chunks.length() == 2 || chunks.length() == 4) {
                append(line);
            }
        }
    });
    connect(m_removeAllProxiesAction, &QAction::triggered, this, &QTextEdit::clear);
}

void ProxiesWidget::showCustomContextMenu(const QPoint& point)
{
    QPoint globalPoint = this->mapToGlobal(point);
    QMenu menu(this);
    menu.addAction(m_pasteProxiesAction);
    menu.addAction(m_removeAllProxiesAction);
    menu.exec(globalPoint);
}

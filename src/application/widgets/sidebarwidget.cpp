#include "sidebarwidget.h"

#include <QAction>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QWidget>


#define ACTION_HEIGHT 90
#define ACTION_DEFAULT_COLOR QColor(100, 100, 100)
#define ACTION_HOVER_COLOR QColor(150, 150, 150)
#define ACTION_SELECTED_COLOR QColor(35, 35, 35)
#define ACTION_TEXT_COLOR QColor(255, 255, 255)


SideBarWidget::SideBarWidget(QWidget *parent) : QWidget(parent),
    m_checkedAction(nullptr), m_hoveredAction(nullptr)
{
    setMouseTracking(true);
}

void SideBarWidget::addAction(QAction *action, bool selected)
{
    m_actions.push_back(action);
    if (selected)
        m_checkedAction = action;
    update();
}

QAction *SideBarWidget::addAction(const QString &text, const QIcon &icon)
{
    QAction* action = new QAction(icon, text, this);
    m_actions.push_back(action);
    update();
    return action;
}

QSize SideBarWidget::minimumSizeHint() const
{
    return ACTION_HEIGHT * QSize(1, m_actions.size());
}

void SideBarWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFont textFont(painter.font());
    textFont.setFamily("Helvetica Neue");
    painter.setFont(textFont);
    painter.fillRect(rect(), ACTION_DEFAULT_COLOR);
    int actionY = 0;
    for (auto action : m_actions) {
        QRect actionRect(0, actionY, event->rect().width(), ACTION_HEIGHT);
        if (action == m_checkedAction) {
            painter.fillRect(actionRect, ACTION_SELECTED_COLOR);
        }
        else if (action == m_hoveredAction) {
            painter.fillRect(actionRect, ACTION_HOVER_COLOR);
        }
        painter.setPen(ACTION_TEXT_COLOR);
        QSize actionTextSize = painter.fontMetrics().size(Qt::TextSingleLine, action->text());
        QRect actionTextRect(QPoint(actionRect.width() / 2 - actionTextSize.width() / 2, actionRect.bottom() - 
actionTextSize.height() - 5), actionTextSize);
        painter.drawText(actionTextRect, Qt::AlignCenter, action->text());
        QRect actionIconRect(0, actionY + 10, actionRect.width(), actionRect.height() - 2 * actionTextRect.height() - 
10);
        QIcon actionIcon(action->icon());
        actionIcon.paint(&painter, actionIconRect);
        actionY += actionRect.height();
    }
}

void SideBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    QAction* action = actionAt(event->pos());
    if (action == nullptr)
    {
        m_hoveredAction = nullptr;
        update();
        return;
    }
    if (action == m_hoveredAction)
    {
        return;
    }
    m_hoveredAction = action;
    update();
    QWidget::mouseMoveEvent(event);
}

void SideBarWidget::mousePressEvent(QMouseEvent *event)
{
    QAction* action = actionAt(event->pos());
    if (action == m_checkedAction)
        return;
    if (action != nullptr)
    {
        m_checkedAction = action;
        update();
        action->trigger();
        Q_EMIT currentActionIndexChanged(m_actions.indexOf(action));
    }
    QWidget::mousePressEvent(event);
}

void SideBarWidget::leaveEvent(QEvent *event)
{
    m_hoveredAction = nullptr;
    update();
    QWidget::leaveEvent(event);
}

QAction *SideBarWidget::actionAt(const QPoint &point) const
{
    int actionY = 0;
    for (QAction* action : m_actions) {
        QRect actionRect(0, actionY, rect().width(), ACTION_HEIGHT);
        if (actionRect.contains(point)) {
            return action;
        }
        actionY += actionRect.height();
    }

    return nullptr;
}

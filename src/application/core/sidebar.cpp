#include <QAction>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QWidget>

#include "sidebar.h"

#define ACTION_HEIGHT 90
#define ACTION_DEFAULT_COLOR QColor(100, 100, 100)
#define ACTION_HOVER_COLOR QColor(150, 150, 150)
#define ACTION_SELECTED_COLOR QColor(35, 35, 35)
#define ACTION_TEXT_COLOR QColor(255, 255, 255)


SideBar::SideBar(QWidget *parent) : QWidget(parent),
    _checkedAction(nullptr), _hoveredAction(nullptr)
{
    setMouseTracking(true);
}

void SideBar::addAction(QAction *action)
{
    _actions.push_back(action);
    update();
}

QAction *SideBar::addAction(const QString &text, const QIcon &icon)
{
    QAction* action = new QAction(icon, text, this);
    _actions.push_back(action);
    update();
    return action;
}

QSize SideBar::minimumSizeHint() const
{
    return ACTION_HEIGHT * QSize(1, _actions.size());
}

void SideBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFont textFont(painter.font());
    textFont.setFamily("Helvetica Neue");
    painter.setFont(textFont);
    painter.fillRect(rect(), ACTION_DEFAULT_COLOR);
    int actionY = 0;
    for (auto action : _actions) {
        QRect actionRect(0, actionY, event->rect().width(), ACTION_HEIGHT);
        if (action == _checkedAction) {
            painter.fillRect(actionRect, ACTION_SELECTED_COLOR);
        }
        else if (action == _hoveredAction) {
            painter.fillRect(actionRect, ACTION_HOVER_COLOR);
        }
        painter.setPen(ACTION_TEXT_COLOR);
        QSize actionTextSize = painter.fontMetrics().size(Qt::TextSingleLine, action->text());
        QRect actionTextRect(QPoint(actionRect.width() / 2 - actionTextSize.width() / 2, actionRect.bottom() - actionTextSize.height() - 5), actionTextSize);
        painter.drawText(actionTextRect, Qt::AlignCenter, action->text());
        QRect actionIconRect(0, actionY + 10, actionRect.width(), actionRect.height() - 2 * actionTextRect.height() - 10);
        QIcon actionIcon(action->icon());
        actionIcon.paint(&painter, actionIconRect);
        actionY += actionRect.height();
    }
}

void SideBar::mouseMoveEvent(QMouseEvent *event)
{
    QAction* action = actionAt(event->pos());
    if (action == nullptr)
    {
        _hoveredAction = nullptr;
        update();
        return;
    }
    if (action == _hoveredAction)
    {
        return;
    }
    _hoveredAction = action;
    update();
    QWidget::mouseMoveEvent(event);
}

void SideBar::mousePressEvent(QMouseEvent *event)
{
    QAction* action = actionAt(event->pos());
    if (action == _checkedAction)
        return;
    _checkedAction = action;
    update();
    emit action->trigger();
    QWidget::mousePressEvent(event);
}

void SideBar::leaveEvent(QEvent *event)
{
    _hoveredAction = nullptr;
    update();
    QWidget::leaveEvent(event);
}

QAction *SideBar::actionAt(const QPoint &point) const
{
    int actionY = 0;
    for (QAction* action : _actions) {
        QRect actionRect(0, actionY, rect().width(), ACTION_HEIGHT);
        if (actionRect.contains(point)) {
            return action;
        }
        actionY += actionRect.height();
    }

    return nullptr;
}

#include <QAction>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QWidget>

#include "sidebar.h"

#define ACTION_HEIGHT 80

SideBar::SideBar(QWidget *parent) : QWidget(parent),
    _checkedAction(nullptr), _hoveredAction(nullptr)
{
    setMouseTracking(true);
}

void SideBar::addAction(QAction *action)
{
    _actions.push_back(action);
    action->setCheckable(true);
    update();
}

QAction *SideBar::addAction(const QString &text, const QIcon &icon)
{
    QAction* action = new QAction(icon, text, this);
    action->setCheckable(true);
    _actions.push_back(action);
    qDebug() << text << " " << icon.name();
    //
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
    painter.fillRect(rect(), QColor(100, 100, 100));
    int actionY = 0;
    for (auto action : _actions) {
        QRect actionRect(0, actionY, event->rect().width(), ACTION_HEIGHT);
        if (action->isChecked()) {
            painter.fillRect(actionRect, QColor(35, 35, 35));
        }
        if (action == _hoveredAction) {
            painter.fillRect(actionRect, QColor(150, 150, 150));
        }
        painter.setPen(QColor(255, 255, 255));
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
    if (action == nullptr) {
        _hoveredAction = nullptr;
        update();
        return;    QAction* action = actionAt(event->pos());
    }
    if (action == _hoveredAction || action->isChecked())
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
    emit action->trigger();
    if (action == _checkedAction)
        return;
    _checkedAction = action;
    update();
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

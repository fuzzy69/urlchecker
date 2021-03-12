#pragma once

#include <QList>
#include <QWidget>

class QAction;

/// Sidebar widget with user defined actions
/// Based on https://stackoverflow.com/a/44327462
class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
    /// Adds an action to sidebar
    void addAction(QAction* action, bool selected = false);
    /// Returns pointer to a created action from given text and icon
    QAction* addAction(const QString& text, const QIcon& icon);
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void leaveEvent(QEvent *event) override;

    QAction* actionAt(const QPoint& point) const;
private:
    QList<QAction*> _actions;
    QAction* _checkedAction;
    QAction* _hoveredAction;
};

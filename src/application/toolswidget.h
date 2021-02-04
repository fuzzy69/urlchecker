#pragma once

// #include <QList>
// #include <QWidget>
#include <QTreeWidget>

// class QAction;
class QTreeWidgetItem;

class ToolsWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ToolsWidget(QWidget *parent = nullptr);

    void addTool(const QIcon &icon, const QString &text);
//     /// Adds an action to sidebar
//     void addAction(QAction* action);
//     /// Returns pointer to a created action from given text and icon
//     QAction* addAction(const QString& text, const QIcon& icon);
//     QSize minimumSizeHint() const;

signals:
    void toolSelected(const QString &toolText);

protected:
//     void paintEvent(QPaintEvent *event) override;
//     void mouseMoveEvent(QMouseEvent *event) override;
//     void mousePressEvent(QMouseEvent * event) override;
//     void leaveEvent(QEvent *event) override;

//     QAction* actionAt(const QPoint& point) const;
private:
//     QList<QAction*> _actions;
//     QAction* _checkedAction;
//     QAction* _hoveredAction;
};

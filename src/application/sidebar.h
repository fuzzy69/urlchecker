#pragma once

#include <QList>
#include <QWidget>

class QAction;

/**
 * Source https://stackoverflow.com/a/44327462
 */
class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
//    void addAction(QAction* action) override;
    void addAction(QAction* action);
    QAction* addAction(const QString& text, const QIcon& icon);
    QSize minimumSizeHint() const;

signals:

public slots:
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

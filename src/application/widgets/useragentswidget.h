#pragma once

#include <QPoint>
#include <QTextEdit>

class UserAgentsWidget : public QTextEdit
{
    Q_OBJECT

public:
    explicit UserAgentsWidget(QTextEdit *parent=nullptr);

public slots:
    void showCustomContextMenu(const QPoint &point);
};

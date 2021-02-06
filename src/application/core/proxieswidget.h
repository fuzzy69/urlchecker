#pragma once

#include <QPoint>
#include <QTextEdit>

class ProxiesWidget : public QTextEdit
{
    Q_OBJECT

public:
    explicit ProxiesWidget(QTextEdit *parent=nullptr);

public slots:
    void showCustomContextMenu(const QPoint &point);
};

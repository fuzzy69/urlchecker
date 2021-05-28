#pragma once

#include <QWidget>


class QHBoxLayout;
class QSpinBox;
class QLabel;
class QGroupBox;
class QGridLayout;
class QCheckBox;
class QTabWidget;
class QHelpEngine;
class QSplitter;

class HelpBrowser;

class HelpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HelpWidget(QWidget *parent=nullptr);

private:
    QHBoxLayout *m_mainLayout;
    QSplitter *m_splitter;
    QTabWidget *m_tabWidget;
    QHelpEngine *m_helpEngine;
    HelpBrowser *m_helpBrowser;
};

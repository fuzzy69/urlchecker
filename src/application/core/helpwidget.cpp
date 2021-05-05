#include <QHBoxLayout>
#include <QTabWidget>
#include <QTextBrowser>
#include <QtHelp/QHelpEngine>
#include <QtHelp/QHelpContentWidget>
#include <QtHelp/QHelpIndexWidget>
#include <QUrl>
#include <QDebug>
#include <QSplitter>

#include "helpwidget.h"
#include "helpbrowser.h"


HelpWidget::HelpWidget(QWidget* parent) : QWidget(parent)
{
    m_mainLayout = new QHBoxLayout(this);
    m_tabWidget = new QTabWidget;
    m_tabWidget->setMinimumWidth(200);
    m_splitter = new QSplitter(Qt::Horizontal);
    auto documentPath = QString("/mnt/ramdisk/docs.qhc");
    auto compiledDocumentPath = QString("/mnt/ramdisk/docs.qch");
    m_helpEngine = new QHelpEngine(documentPath, this);
    auto documentNamespace = QHelpEngineCore::namespaceName(documentPath);
    m_helpEngine->registerDocumentation(compiledDocumentPath);
    m_helpEngine->setupData();
    m_helpBrowser = new HelpBrowser(m_helpEngine);
    m_helpBrowser->setSource(QUrl("qthelp://docs.com.urlchecker/doc/index.html"));
    m_tabWidget->addTab(m_helpEngine->contentWidget(), "Contents");
    m_tabWidget->addTab(m_helpEngine->indexWidget(), "Index");
    m_splitter->addWidget(m_tabWidget);
    m_splitter->addWidget(m_helpBrowser);
    m_mainLayout->addWidget(m_splitter);
    m_tabWidget->resize(300, m_tabWidget->height());

    connect(m_helpEngine->contentWidget(), &QHelpContentWidget::linkActivated, m_helpBrowser, &HelpBrowser::setSource);
    connect(m_helpEngine->indexWidget(), &QHelpIndexWidget::linkActivated, m_helpBrowser, &HelpBrowser::setSource);
}

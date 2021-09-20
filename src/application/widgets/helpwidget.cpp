#include "helpwidget.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QTextBrowser>
#include <QUrl>
#include <QtHelp/QHelpContentWidget>
#include <QtHelp/QHelpEngine>
#include <QtHelp/QHelpIndexWidget>

#include "../icons.h"
#include "helpbrowser.h"

HelpWidget::HelpWidget(QWidget* parent)
    : QWidget(parent)
{
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    m_tabWidget = new QTabWidget;
    m_tabWidget->setMinimumWidth(200);
    m_splitter = new QSplitter(Qt::Horizontal);
    auto documentPath = QString(QDir(QApplication::applicationDirPath()).filePath("docs/help.qhc"));
    auto compiledDocumentPath = QString(QDir(QApplication::applicationDirPath()).filePath("docs/help.qch"));
    m_helpEngine = new QHelpEngine(documentPath, this);
    auto documentNamespace = QHelpEngineCore::namespaceName(documentPath);
    m_helpEngine->registerDocumentation(compiledDocumentPath);
    m_helpBrowser = new HelpBrowser(m_helpEngine);

    if (documentNamespace.isEmpty())
        qWarning() << "Document namespace is empty!";
    if (!m_helpEngine->registeredDocumentations().contains(documentNamespace))
        qWarning() << "Document namespace is not reistered!";

    m_tabWidget->addTab(m_helpEngine->contentWidget(), QIcon(ICON_BOOK_OPEN_LIST), tr("Contents"));
    m_tabWidget->addTab(m_helpEngine->indexWidget(), QIcon(ICON_BOOK_OPEN_BOOKMARK), tr("Index"));
    m_splitter->addWidget(m_tabWidget);
    m_splitter->addWidget(m_helpBrowser);
    m_mainLayout->addWidget(m_splitter);
    m_splitter->setSizes(QList<int>({ 300, 1000 }));

    // connect(m_helpEngine->contentWidget(), &QHelpContentWidget::linkActivated, m_helpBrowser, &HelpBrowser::setSource);
    // connect(m_helpEngine->indexWidget(), &QHelpIndexWidget::linkActivated, m_helpBrowser, &HelpBrowser::setSource);
}

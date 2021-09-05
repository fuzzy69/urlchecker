#include "settingswidget.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QDir>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSpinBox>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "../../config.h"
#include "../../core/settings.h"
#include "../../icons.h"
#include "../../texts.h"

#include "connectionsettingspage.h"
#include "generalsettingspage.h"
#include "proxiessettingspage.h"
#include "useragentssettingspage.h"

#include "../../tools/scrapehtml/settingspage.h"
#include "../../tools/scrapeimages/scrapeimagessettingspage.h"
#include "../../tools/scrapelinks/scrapelinkssettingspage.h"
#include "../../tools/scrapesitemaps/scrapesitemapssettingspage.h"

// TODO: Remove tools include dependecies

SettingsWidget::SettingsWidget(QWidget* parent)
    : QWidget(parent)
    , m_itemsMap(QMap<QString, QListWidgetItem*>())
{
    m_categoriesListWidget = new QListWidget;
    m_categoriesListWidget->setViewMode(QListView::IconMode);
    m_categoriesListWidget->setIconSize(QSize(16, 16));
    m_categoriesListWidget->setMaximumWidth(200);
    m_categoriesListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    m_contentsStackedWidget = new QStackedWidget;

    // Icons
    addItem(QIcon(ICON_DASHBOARD), QStringLiteral("General"), new GeneralSettingsPage);
    addItem(QIcon(ICON_NETWORK_IP), QStringLiteral("Connection"), new ConnectionSettingsPage);
    addItem(QIcon(ICON_USER_SILHOUETTE_QUESTION), QStringLiteral("User Agents"), new UserAgentsSettingsPage);
    addItem(QIcon(ICON_MASK), QStringLiteral("Proxies"), new ProxiesSettingsPage);

    addItem(QIcon(ICON_IMAGES), QStringLiteral("Scrape Images"), new ScrapeImagesSettingsPage);
    addItem(QIcon(ICON_CHAIN), QStringLiteral("Scrape Links"), new ScrapeLinksSettingsPage);
    addItem(QIcon(ICON_SITEMAP), QStringLiteral("Scrape Sitemaps"), new ScrapeSitemapsSettingsPage);
    addItem(QIcon(ICON_DOCUMENT_NUMBER), QStringLiteral("Scrape HTML"), new ScrapeHtmlSettingsPage);

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_categoriesListWidget);
    m_mainLayout->addWidget(m_contentsStackedWidget);

    connect(m_categoriesListWidget, &QListWidget::currentRowChanged, m_contentsStackedWidget, &QStackedWidget::setCurrentIndex);
}

void SettingsWidget::addItem(const QIcon& icon, const QString& text, QWidget* pageWidget)
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setIcon(icon);
    item->setText(text);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setSizeHint(QSize(200, 40));
    m_categoriesListWidget->addItem(item);
    m_contentsStackedWidget->addWidget(pageWidget);
    m_itemsMap.insert(text, item);
    m_items.append(text);
}

int SettingsWidget::itemIndex(const QString& text) const
{
    return m_items.indexOf(text);
}

bool SettingsWidget::setCurrentSettingsPage(const QString& text)
{
    const int settingsIndex = itemIndex(text);
    if (settingsIndex > -1 and settingsIndex < m_contentsStackedWidget->count()) {
        if (m_itemsMap.contains(text)) {
            m_contentsStackedWidget->setCurrentIndex(settingsIndex);
            m_categoriesListWidget->setCurrentItem(m_itemsMap[text]);
            return true;
        }
    }

    return false;
}

void SettingsWidget::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event)
}

void SettingsWidget::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
}

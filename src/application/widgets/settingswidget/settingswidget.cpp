﻿#include "settingswidget.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QDir>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>

#include "../../config.h"
#include "../../texts.h"
#include "../../core/settings.h"
#include "../../icons.h"

#include "generalsettingspage.h"
#include "connectionsettingspage.h"
#include "useragentssettingspage.h"
#include "proxiessettingspage.h"

#include "../../tools/scrapelinks/scrapelinkssettingspage.h"

SettingsWidget::SettingsWidget ( QWidget* parent ) :
    QWidget(parent), m_itemsMap(QMap<QString, QListWidgetItem*>())
{
//    QDir applicationDir(QApplication::applicationDirPath());
//    m_settingsFilePath = applicationDir.absoluteFilePath("settings.ini");

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
    addItem(QIcon(ICON_CHAIN), QStringLiteral("Scrape Links"), new ScrapeLinksSettingsPage);
//    QListWidgetItem* generalItem = new QListWidgetItem;
//    generalItem->setIcon(QIcon(ICON_DASHBOARD));
//    generalItem->setText(QStringLiteral("General"));
//    generalItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    generalItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//    generalItem->setSizeHint(QSize(200, 40));
//    m_categoriesListWidget->addItem(generalItem);
//    QListWidgetItem* connectionItem = new QListWidgetItem;
//    connectionItem->setIcon(QIcon(ICON_NETWORK_IP));
//    connectionItem->setText(QStringLiteral("Connection"));
//    connectionItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    connectionItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//    connectionItem->setSizeHint(QSize(200, 40));
//    m_categoriesListWidget->addItem(connectionItem);
//    QListWidgetItem* userAgentsItem = new QListWidgetItem;
//    userAgentsItem->setIcon(QIcon(ICON_USER_SILHOUETTE_QUESTION));
//    userAgentsItem->setText(QStringLiteral("User Agents"));
//    userAgentsItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    userAgentsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//    userAgentsItem->setSizeHint(QSize(200, 40));
//    m_categoriesListWidget->addItem(userAgentsItem);
//    QListWidgetItem* proxiesItem = new QListWidgetItem;
//    proxiesItem->setIcon(QIcon(ICON_MASK));
//    proxiesItem->setText(QStringLiteral("Proxies"));
//    proxiesItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    proxiesItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//    proxiesItem->setSizeHint(QSize(200, 40));
//    m_categoriesListWidget->addItem(proxiesItem);
//    QListWidgetItem* scrapeLinksItem = new QListWidgetItem;
//    scrapeLinksItem->setIcon(QIcon(ICON_CHAIN));
//    scrapeLinksItem->setText(QStringLiteral("Scrape Links"));
//    scrapeLinksItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    scrapeLinksItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//    scrapeLinksItem->setSizeHint(QSize(200, 40));
//    m_categoriesListWidget->addItem(scrapeLinksItem);

//    m_contentsStackedWidget->addWidget(new GeneralSettingsPage);
//    m_contentsStackedWidget->addWidget(new ConnectionSettingsPage);
//    m_contentsStackedWidget->addWidget(new UserAgentsSettingsPage);
//    m_contentsStackedWidget->addWidget(new ProxiesSettingsPage);
//    m_contentsStackedWidget->addWidget(new ScrapeLinksSettingsPage);
    m_mainLayout = new QHBoxLayout(this);

    m_mainLayout->addWidget(m_categoriesListWidget);
    m_mainLayout->addWidget(m_contentsStackedWidget);
//    m_connectionGroupBox = new QGroupBox("Connection");
//    m_proxiesGroupBox = new QGroupBox("Proxies");
//    m_threadsLabel = new QLabel("Thread count");
//    m_timeoutLabel = new QLabel("Timeout (seconds)");
//    m_verifySslLabel = new QLabel("Verify SSL");
//    m_verifySslCheckBox = new QCheckBox;
//    m_useProxiesLabel = new QLabel("Use proxies");
//    m_useProxiesCheckBox = new QCheckBox;
//    m_threadsSpinBox = new QSpinBox;
//    m_threadsSpinBox->setRange(1, 10);
//    m_timeoutSpinBox = new QSpinBox;
//    m_timeoutSpinBox->setRange(1, 60);
//    m_connectionLayout = new QGridLayout(m_connectionGroupBox);
//    m_connectionLayout->addWidget(m_threadsLabel, 0, 0);
//    m_connectionLayout->addWidget(m_threadsSpinBox, 0, 1);
//    m_connectionLayout->addWidget(m_timeoutLabel, 1, 0);
//    m_connectionLayout->addWidget(m_timeoutSpinBox, 1, 1);
//    m_connectionLayout->addWidget(m_verifySslLabel, 2, 0);
//    m_connectionLayout->addWidget(m_verifySslCheckBox, 2, 1);

//    m_proxiesLayout = new QGridLayout(m_proxiesGroupBox);
//    m_proxiesLayout->addWidget(m_useProxiesLabel, 0, 0);
//    m_proxiesLayout->addWidget(m_useProxiesCheckBox, 0, 1);

    connect(m_categoriesListWidget, &QListWidget::currentRowChanged, m_contentsStackedWidget, &QStackedWidget::setCurrentIndex);
//    m_mainLayout->addWidget(m_connectionGroupBox);
//    m_mainLayout->addWidget(m_proxiesGroupBox);
    //    m_mainLayout->addStretch(0);
}

void SettingsWidget::addItem(const QIcon &icon, const QString &text, QWidget *pageWidget)
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

int SettingsWidget::itemIndex(const QString &text) const
{
    return m_items.indexOf(text);
}

bool SettingsWidget::setCurrentSettingsPage(const QString &text)
{
    const int settingsIndex = itemIndex(text);
    if (settingsIndex > -1 and settingsIndex < m_contentsStackedWidget->count())
    {
        if (m_itemsMap.contains(text))
        {
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
//    Settings::instance().setValue( TEXT_THREADS, QVariant(m_threadsSpinBox->value()));
//    Settings::instance().setValue( TEXT_TIMEOUT, QVariant(m_timeoutSpinBox->value()));
//    Settings::instance().setValue( TEXT_VERIFY_SSL, QVariant(m_verifySslCheckBox->isChecked()));
//    Settings::instance().setValue( TEXT_USE_PROXIES, QVariant(m_useProxiesCheckBox->isChecked()));
}

void SettingsWidget::showEvent ( QShowEvent* event )
{
    Q_UNUSED(event)
//    m_threadsSpinBox->setValue(Settings::instance().value(QStringLiteral(TEXT_THREADS)).toInt());
//    m_timeoutSpinBox->setValue(Settings::instance().value( TEXT_TIMEOUT ).toInt());
//    m_verifySslCheckBox->setChecked(Settings::instance().value( TEXT_VERIFY_SSL ).toBool());
//    m_useProxiesCheckBox->setChecked(Settings::instance().value( TEXT_USE_PROXIES ).toBool());
}

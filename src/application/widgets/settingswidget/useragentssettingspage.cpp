#include "useragentssettingspage.h"
#include "ui_useragentssettings.h"

#include "../../config.h"
#include "../../core/settings.h"
#include "../../icons.h"
#include "../../texts.h"

using my::browser::UserAgentsManager;

UserAgentsSettingsPage::UserAgentsSettingsPage(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui_UserAgentsSettings)
{
    m_ui->setupUi(this);
}

UserAgentsSettingsPage::~UserAgentsSettingsPage()
{
    delete m_ui;
}

void UserAgentsSettingsPage::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event)
    const auto defaultUserAgent = m_ui->globalUserAgentLineEdit->text();
    Settings::instance().setValue(USER_AGENT, QVariant(defaultUserAgent));
    Settings::instance().setValue(QStringLiteral("userAgentsStrategy"), QVariant(static_cast<int>(rotationStrategy())));
    UserAgentsManager<QString>::instance().set_default_user_agent(defaultUserAgent);
    UserAgentsManager<QString>::instance().set_rotation_strategy(rotationStrategy());
}

void UserAgentsSettingsPage::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    auto userAgent = Settings::instance().value(QStringLiteral(USER_AGENT)).toString();
    if (userAgent.isEmpty())
        userAgent = QStringLiteral(USER_AGENT);
    m_ui->globalUserAgentLineEdit->setText(userAgent);
    setRotationStrategy(static_cast<RotationStrategy>(Settings::instance().value(QStringLiteral("userAgentsStrategy")).toInt()));
}

void UserAgentsSettingsPage::setRotationStrategy(RotationStrategy rotationStrategy)
{
    switch (rotationStrategy) {
    case RotationStrategy::DEFAULT_USER_AGENT:
        m_ui->useGlobalUserAgentRadioButton->setChecked(true);
        break;
    case RotationStrategy::RANDOM_USER_AGENT:
        m_ui->useRandomUserAgentRadioButton->setChecked(true);
        break;
    case RotationStrategy::TIE_USER_AGENT_TO_PROXY:
        m_ui->tieUserAgentToProxyRadioButton->setChecked(true);
        break;
    }
}

RotationStrategy UserAgentsSettingsPage::rotationStrategy()
{
    if (m_ui->useGlobalUserAgentRadioButton->isChecked())
        return RotationStrategy::DEFAULT_USER_AGENT;
    else if (m_ui->useRandomUserAgentRadioButton->isChecked())
        return RotationStrategy::RANDOM_USER_AGENT;
    else
        return RotationStrategy::TIE_USER_AGENT_TO_PROXY;
}

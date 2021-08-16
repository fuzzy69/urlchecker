#include "requests.h"

#include "my/httpproxy.h"
#include "my/proxymanager.h"
#include "my/browserutils.h"

#include "../config.h"
#include "../constants.h"

using my::network::HttpProxy;
using my::network::ProxyManager;
using my::browser::UserAgentsManager;

Requests::Requests(const QVariantMap& settings) : m_settings(settings)
{
    m_timeout = m_settings["timeout"].toInt() * MILLIS_IN_SECOND;
    m_verifySsl = m_settings["verifySsl"].toBool();
    m_useProxies = m_settings["useProxies"].toBool();
}

cpr::Response Requests::get(const std::string& url)
{
    auto headers = cpr::Header{
//        {"user-agent", UserAgentsManager<QString>::instance().get_random().toStdString()}
        {"user-agent", UserAgentsManager<QString>::instance().get_user_agent().toStdString()}
    };
    if (m_useProxies)
    {
        auto proxy = ProxyManager::instance().get_proxy();
        auto proxies = cpr::Proxies{
            {"http", proxy.to_text()},
            {"https", proxy.to_text()}
        };
        return cpr::Get(cpr::Url{url}, cpr::Timeout{m_timeout}, headers, proxies, cpr::VerifySsl{m_verifySsl});
    }
    else
    {
        return cpr::Get(cpr::Url{url}, cpr::Timeout{m_timeout}, headers, cpr::VerifySsl{m_verifySsl});
    }
}

cpr::Response Requests::head(const std::string& url)
{
    auto headers = cpr::Header{
//        {"user-agent", UserAgentsManager<QString>::instance().get_random().toStdString()}
        {"user-agent", UserAgentsManager<QString>::instance().get_user_agent().toStdString()}
    };
    if (m_useProxies)
    {
        auto proxy = ProxyManager::instance().get_proxy();
        auto proxies = cpr::Proxies{
            {"http", proxy.to_text()},
            {"https", proxy.to_text()}
        };
        return cpr::Head(cpr::Url{url}, cpr::Timeout{m_timeout}, headers, proxies, cpr::VerifySsl{m_verifySsl});
    }
    else
    {
        return cpr::Head(cpr::Url{url}, cpr::Timeout{m_timeout}, headers, cpr::VerifySsl{m_verifySsl});
    }
}

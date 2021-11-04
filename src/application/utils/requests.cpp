#include <fstream>

#include "requests.h"

#include "../../common/httpproxy.h"
#include "../../common/proxymanager.h"
#include "../../common/useragentmanager.h"

#include "../config.h"
#include "../constants.h"

using common::browser::UserAgentManager;
using common::network::HttpProxy;
using common::network::ProxyManager;

Requests::Requests(const QVariantMap& settings)
    : m_settings(settings)
{
    m_timeout = m_settings["timeout"].toInt() * MILLIS_IN_SECOND;
    m_verifySsl = m_settings["verifySsl"].toBool();
    m_useProxies = m_settings["useProxies"].toBool();
}

cpr::Response Requests::get(const std::string& url)
{
    auto headers = cpr::Header {
        { "user-agent", UserAgentManager::self().get_user_agent() }
    };
    if (m_useProxies) {
        auto proxy = ProxyManager::self().get_proxy();
        auto proxies = cpr::Proxies {
            { "http", proxy.to_text() },
            { "https", proxy.to_text() }
        };
        return cpr::Get(cpr::Url { url }, cpr::Timeout { m_timeout }, headers, proxies, cpr::VerifySsl { m_verifySsl });
    } else {
        return cpr::Get(cpr::Url { url }, cpr::Timeout { m_timeout }, headers, cpr::VerifySsl { m_verifySsl });
    }
}

cpr::Response Requests::head(const std::string& url)
{
    auto headers = cpr::Header {
        { "user-agent", UserAgentManager::self().get_user_agent() }
    };
    if (m_useProxies) {
        auto proxy = ProxyManager::self().get_proxy();
        auto proxies = cpr::Proxies {
            { "http", proxy.to_text() },
            { "https", proxy.to_text() }
        };
        return cpr::Head(cpr::Url { url }, cpr::Timeout { m_timeout }, headers, proxies, cpr::VerifySsl { m_verifySsl });
    } else {
        return cpr::Head(cpr::Url { url }, cpr::Timeout { m_timeout }, headers, cpr::VerifySsl { m_verifySsl });
    }
}

cpr::Response Requests::download(const std::string& url, const std::string& filePath)
{
    auto ofstream = std::ofstream(filePath);
    auto headers = cpr::Header {
        { "user-agent", UserAgentManager::self().get_user_agent() }
    };
    auto session = cpr::Session();
    session.SetUrl(cpr::Url(url));
    session.SetTimeout(m_timeout);
    session.SetVerifySsl(m_verifySsl);

    return session.Download(ofstream);
}

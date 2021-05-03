#include "my/proxymanager.h"
#include "my/random.h"


using my::network::HttpProxy;
using my::network::ProxyManager;
using my::misc::Random;

ProxyManager::ProxyManager()
{

};

ProxyManager & ProxyManager::instance()
{
    static ProxyManager proxyManager;

    return proxyManager;
}

void ProxyManager::add_proxy(const HttpProxy &httpProxy)
{
    m_proxies.push_back(httpProxy);
}

HttpProxy& ProxyManager::get_proxy()
{
    return m_proxies[Random::instance().get(0, m_proxies.size() - 1)];
}


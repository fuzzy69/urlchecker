#include "proxymanager.h"
#include "random.h"

using common::misc::Random;
using common::network::HttpProxy;
using common::network::ProxyManager;

ProxyManager::ProxyManager() {

};

ProxyManager& ProxyManager::self()
{
    static ProxyManager proxyManager;

    return proxyManager;
}

void ProxyManager::add_proxy(const HttpProxy& httpProxy)
{
    m_proxies.push_back(httpProxy);
}

HttpProxy& ProxyManager::get_proxy()
{
    return m_proxies[Random::instance().get(0, m_proxies.size() - 1)];
}

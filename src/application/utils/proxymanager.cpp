#include "proxymanager.h"
#include "rand.h"


ProxyManager::ProxyManager()
{

};

void ProxyManager::add_proxy(const HttpProxy &httpProxy)
{
    m_proxies.push_back(httpProxy);
}

HttpProxy& ProxyManager::get_proxy()
{
    return m_proxies[Rand::instance().get(0, m_proxies.size() - 1)];
}
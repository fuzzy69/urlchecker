#pragma once

#include <vector>

#include "httpproxy.h"


class ProxyManager final
{
public:
    ProxyManager(const ProxyManager &other) = delete;
    void operator=(const ProxyManager &other) = delete;

    static ProxyManager& instance();

    void add_proxy(const HttpProxy &httpProxy);
    HttpProxy& get_proxy();
    size_t count() const { return m_proxies.size(); };

private:
    ProxyManager();

    std::vector<HttpProxy> m_proxies;
};

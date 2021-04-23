#pragma once

#include <vector>

#include "httpproxy.h"


class ProxyManager final
{
public:
    explicit ProxyManager();

    void add_proxy(const HttpProxy &httpProxy);
    HttpProxy& get_proxy();
    size_t count() const { return m_proxies.size(); };

private:
    std::vector<HttpProxy> m_proxies;
};

#include <regex>
#include <string>
#include <vector>

#include "httpproxy.h"


namespace common
{
namespace network
{
HttpProxy::HttpProxy(std::string ip, unsigned short port, std::string username, std::string password, bool ssl) : 
m_ip(ip), m_port(port), m_username(username), m_password(password), m_type(ssl? HttpProxyType::HTTPS : 
HttpProxyType::HTTP)
{
}

std::optional<HttpProxy> HttpProxy::from_text(std::string text)
{
    std::regex regex(R"(([^.]+.[^.]+.[^.]+.[^:]+):(\d{2,5})(?::(.*):(.*))?)");
    std::smatch match;
    if (std::regex_search(text, match, regex))
    {
        int port = std::stoi(match[2]);
        return HttpProxy(match[1], port, match[3], match[4]);
    }
    else
    {
        return {};
    }
}

HttpProxy::operator std::string()
{
    std::ostringstream stream;
    if (is_public())
        stream << ip() << ":" << port();
    else
        stream << ip() << ":" << port() << ":" << username() << ":" << password();

    return stream.str();
}

std::ostream& operator<<(std::ostream& stream, const HttpProxy& httpProxy)
{
    if (httpProxy.is_public())
        stream << "HttpProxy(\"" << httpProxy.ip() << "\", \"" << httpProxy.port() << "\")";
    else
        stream << "HttpProxy(\"" << httpProxy.ip() << "\", \"" << httpProxy.port() << "\", \"" << httpProxy.username() 
<< "\", \"" << httpProxy.password() << "\")";

    return stream;
}

std::string HttpProxy::to_text() const
{
    std::ostringstream stream;
    if (is_public())
        stream << ((m_type == HttpProxyType::HTTPS)? "https" : "http") << "://" << ip() << ":" << port();
    else
        stream << ((m_type == HttpProxyType::HTTPS)? "https" : "http") << "://" << username() << ":" << password() << 
"@" << ip() << ":" << port();

    return stream.str();
}
}
}

#include "httpproxy.h"


// HttpProxy::HttpProxy ( char* ip, unsigned short port, char* username, char* password, bool ssl ) : m_ip(ip), m_port(port), m_username(username), m_password(password), m_type(ssl? HttpProxyType:HTTPS : HttpProxyType:HTTP)
// {
//     
// }
// 
// HttpProxy HttpProxy::fromText ( char* text )
// {
//     return HttpProxy("127.0.0.1", 80);
// }
HttpProxy::HttpProxy(std::string ip, unsigned short port, std::string username, std::string password, bool ssl) : 
m_ip(ip), m_port(port), m_username(username), m_password(password), m_type(ssl? HttpProxyType::HTTPS : HttpProxyType::HTTP)
{
}

HttpProxy HttpProxy::fromText(std::string text)
{
    return HttpProxy("127.0.0.1", 80);
}

std::string HttpProxy::ip() const
{
    return m_ip;
}

unsigned short HttpProxy::port() const
{
    return m_port;
}

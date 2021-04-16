#pragma once

#include <string>


enum HttpProxyType
{
    HTTP,
    HTTPS
};

struct HttpProxy
{
    HttpProxy(std::string ip, unsigned short port, std::string username="", std::string password="", bool ssl=false);

    HttpProxy static fromText(std::string text);

    std::string ip() const;
    unsigned short port() const;

private:
    std::string m_ip;
    unsigned short m_port;
    std::string m_username;
    std::string m_password;
    HttpProxyType m_type;
};

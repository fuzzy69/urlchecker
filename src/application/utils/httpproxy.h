#pragma once

#include <iostream>
#include <optional>
#include <string>


enum HttpProxyType
{
    HTTP,
    HTTPS
};

struct HttpProxy final
{
    HttpProxy(std::string ip, unsigned short port, std::string username="", std::string password="", bool ssl=false);

    std::optional<HttpProxy> static from_text(std::string text);

    std::string ip() const { return m_ip; };
    unsigned short port() const { return m_port; };
    std::string username() const { return m_username; };
    std::string password() const { return m_password; };

    bool is_public() const { return m_username.length() == 0 && m_password.length() == 0; };

    bool operator==(const HttpProxy& rhs) const { return m_ip == rhs.ip() && m_port == rhs.port() && m_username == rhs.username() && m_password == rhs.password(); };

    friend std::ostream& operator<<(std::ostream& stream, const HttpProxy& httpProxy);

private:
    std::string m_ip;
    unsigned short m_port;
    std::string m_username;
    std::string m_password;
    HttpProxyType m_type;
};

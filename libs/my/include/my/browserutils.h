#pragma once

#include <algorithm>
#include <vector>

#include "my/random.h"


namespace my
{
namespace browser
{
using my::misc::Random;

template<typename T>
class UserAgentsManager final
{
public:
    UserAgentsManager(const UserAgentsManager &other) = delete;
    void operator=(const UserAgentsManager &other) = delete;

    static UserAgentsManager& instance()
    {
        static UserAgentsManager userAgentsManager;

        return userAgentsManager;
    }

    void add_user_agent(const T& userAgent)
    {
        if (!std::any_of(m_userAgents.begin(), m_userAgents.end(), [&](const T& element) { return element == userAgent; 
}))
            m_userAgents.push_back(userAgent);
    };
    T get_random() const
    {
        if (m_userAgents.size() == 0)
            return {};
        else if (m_userAgents.size() == 1)
            return m_userAgents.at(0);
        else
            return m_userAgents.at(Random::instance().get(0, m_userAgents.size() - 1));
    };
    size_t count() const { return m_userAgents.size(); };

private:
    UserAgentsManager() : m_userAgents() {};

    std::vector<T> m_userAgents;
};
}
}

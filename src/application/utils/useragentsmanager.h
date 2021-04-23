#pragma once

#include <vector>

#include "rand.h"

template<typename T>
class UserAgentsManager
{
public:
    explicit UserAgentsManager() : m_userAgents()
    {
    };

    void add_user_agent(const T& userAgent)
    {
        // TODO: Ignore adding duplicates
        m_userAgents.push_back(userAgent);
    };
    T get_random() const { return m_userAgents.at(Rand::instance().get(0, m_userAgents.length() - 1)); };
    size_t count() const { return m_userAgents.size(); };

private:
    std::vector<T> m_userAgents;
};

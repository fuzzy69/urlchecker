#pragma once

#include <algorithm>
#include <vector>

#include "my/random.h"


namespace my
{
namespace browser
{
using my::misc::Random;

enum class RotationStrategy
{
    DEFAULT_USER_AGENT,
    RANDOM_USER_AGENT,
    TIE_USER_AGENT_TO_PROXY
};

template<typename T>
class UserAgentsManager final
{
public:
    UserAgentsManager(const UserAgentsManager &other) = delete;
    void operator=(const UserAgentsManager &other) = delete;
    ///
    static UserAgentsManager& instance()
    {
        static UserAgentsManager userAgentsManager;

        return userAgentsManager;
    }
    ///
    void add_user_agent(const T& userAgent)
    {
        if (!std::any_of(m_userAgents.begin(), m_userAgents.end(), [&](const T& element) { return element == userAgent; 
}))
            m_userAgents.push_back(userAgent);
    };
    ///
    T get_random() const
    {
        if (m_userAgents.size() == 0)
            return {};
        else if (m_userAgents.size() == 1)
            return m_userAgents.at(0);
        else
            return m_userAgents.at(Random::instance().get(0, m_userAgents.size() - 1));
    };
    T get_user_agent()
    {
        switch (m_rotation_strategy)
        {
            case RotationStrategy::DEFAULT_USER_AGENT:
                return m_default_user_agent;
            case RotationStrategy::RANDOM_USER_AGENT:
                return get_random();
            case RotationStrategy::TIE_USER_AGENT_TO_PROXY:
                return get_random();
            default:
                return m_default_user_agent;
        }
    }
    ///
    size_t count() const { return m_userAgents.size(); };
    ///
    void set_rotation_strategy(RotationStrategy rotation_strategy)
    {
        m_rotation_strategy = rotation_strategy;
    }
    ///
    RotationStrategy rotation_strategy() const
    {
        return m_rotation_strategy;
    }
    ///
    void set_default_user_agent(T user_agent)
    {
        m_default_user_agent = user_agent;
    }

private:
    UserAgentsManager() : m_userAgents(), m_rotation_strategy(RotationStrategy::DEFAULT_USER_AGENT), m_default_user_agent(T()) {};

    std::vector<T> m_userAgents;
    RotationStrategy m_rotation_strategy;
    T m_default_user_agent;
};
}
}

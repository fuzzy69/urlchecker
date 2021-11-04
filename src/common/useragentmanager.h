#pragma once

#include <algorithm>
#include <vector>
#include <string>

#include "random.h"


namespace common
{
namespace browser
{
using common::misc::Random;

enum class RotationStrategy
{
    DEFAULT_USER_AGENT,
    RANDOM_USER_AGENT,
    TIE_USER_AGENT_TO_PROXY
};

class UserAgentManager final
{
public:
    UserAgentManager(const UserAgentManager &other) = delete;
    void operator=(const UserAgentManager &other) = delete;

    static UserAgentManager& self();
    ///
    void add_user_agent(const std::string& user_agent);
    ///
    std::string get_random() const;
    ///
    std::string get_user_agent() const;
    ///
    size_t count() const { return m_user_agents.size(); }
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
        void set_default_user_agent(const std::string& user_agent)
        {
            m_default_user_agent = user_agent;
        }

private:
    UserAgentManager();

    std::vector<std::string> m_user_agents;
    RotationStrategy m_rotation_strategy;
    std::string m_default_user_agent;
};
}
}

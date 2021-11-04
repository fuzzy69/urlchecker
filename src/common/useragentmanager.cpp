#include "useragentmanager.h"
#include "random.h"

using common::browser::UserAgentManager;
using common::misc::Random;

UserAgentManager::UserAgentManager()
    : m_user_agents()
    , m_rotation_strategy(RotationStrategy::DEFAULT_USER_AGENT)
    , m_default_user_agent("")
{
}

UserAgentManager& UserAgentManager::self()
{
    static UserAgentManager userAgentManager;

    return userAgentManager;
}

void UserAgentManager::add_user_agent(const std::string& user_agent)
{
    if (!std::any_of(m_user_agents.begin(), m_user_agents.end(), [&](const std::string& element) {
            return element == user_agent;
        }))
        m_user_agents.push_back(user_agent);
}

std::string UserAgentManager::get_random() const
{
    if (m_user_agents.size() == 0)
        return {};
    else if (m_user_agents.size() == 1)
        return m_user_agents.at(0);
    else
        return m_user_agents.at(Random::instance().get(0, m_user_agents.size() - 1));
}

std::string UserAgentManager::get_user_agent() const
{
    switch (m_rotation_strategy) {
    case RotationStrategy::DEFAULT_USER_AGENT:
        return m_default_user_agent;
    case RotationStrategy::RANDOM_USER_AGENT:
        return get_random();
    case RotationStrategy::TIE_USER_AGENT_TO_PROXY:
        return get_random();
    }

    return m_default_user_agent;
}

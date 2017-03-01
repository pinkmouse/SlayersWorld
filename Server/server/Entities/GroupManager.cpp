#include "GroupManager.hpp"
#include "Player.hpp"

GroupManager::GroupManager()
{
}

GroupManager::~GroupManager()
{
}

void GroupManager::AddPlayerToGroup(eGroupType p_GroupType, const std::string & p_GroupName, Player* p_Player)
{
    if (m_GroupList.find(p_GroupType) != m_GroupList.end()) ///< if already exist
    {
        if (m_GroupList[p_GroupType].find(p_GroupName) != m_GroupList[p_GroupType].end())
        {
            if (std::find(m_GroupList[p_GroupType][p_GroupName].begin(), m_GroupList[p_GroupType][p_GroupName].end(), p_Player) != m_GroupList[p_GroupType][p_GroupName].end())
                return;
        }
    }

    m_GroupList[p_GroupType][p_GroupName].push_back(p_Player);
}

void GroupManager::RemovePlayerFromGroup(eGroupType p_GroupType, const std::string & p_GroupName, Player* p_Player)
{
    if (m_GroupList.find(p_GroupType) == m_GroupList.end())
        return;

    if (m_GroupList[p_GroupType].find(p_GroupName) == m_GroupList[p_GroupType].end())
        return;

    std::vector<Player*>::iterator l_It = std::find(m_GroupList[p_GroupType][p_GroupName].begin(), m_GroupList[p_GroupType][p_GroupName].end(), p_Player);
    if (l_It == m_GroupList[p_GroupType][p_GroupName].end())
        return;

    m_GroupList[p_GroupType][p_GroupName].erase(l_It);

    if (m_GroupList[p_GroupType][p_GroupName].empty())
        m_GroupList[p_GroupType].erase(m_GroupList[p_GroupType].find(p_GroupName));

    if (m_GroupList[p_GroupType].empty())
        m_GroupList.erase(m_GroupList.find(p_GroupType));
}

void GroupManager::RemovePlayerFromAllGroupType(eGroupType p_GroupType, Player* p_Player)
{
    ;
}

std::vector< Player* >* GroupManager::GetPlayerForGroup(eGroupType p_GroupType, const std::string & p_GroupName)
{
    if (m_GroupList.find(p_GroupType) == m_GroupList.end())
        return nullptr;

    if (m_GroupList[p_GroupType].find(p_GroupName) == m_GroupList[p_GroupType].end())
        return nullptr;

    return &m_GroupList[p_GroupType][p_GroupName];
}

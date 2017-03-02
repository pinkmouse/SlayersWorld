#include "GroupManager.hpp"
#include "Unit.hpp"

GroupManager::GroupManager()
{
}

GroupManager::~GroupManager()
{
}

void GroupManager::AddUnitToGroup(eGroupType p_GroupType, const std::string & p_GroupName, Unit* p_Unit)
{
    if (m_GroupList.find(p_GroupType) != m_GroupList.end()) ///< if already exist
    {
        if (m_GroupList[p_GroupType].find(p_GroupName) != m_GroupList[p_GroupType].end())
        {
            if (std::find(m_GroupList[p_GroupType][p_GroupName].begin(), m_GroupList[p_GroupType][p_GroupName].end(), p_Unit) != m_GroupList[p_GroupType][p_GroupName].end())
                return;
        }
    }

    m_GroupList[p_GroupType][p_GroupName].push_back(p_Unit);
}

void GroupManager::RemoveUnitFromGroup(eGroupType p_GroupType, const std::string & p_GroupName, Unit* p_Unit)
{
    if (m_GroupList.find(p_GroupType) == m_GroupList.end())
        return;

    if (m_GroupList[p_GroupType].find(p_GroupName) == m_GroupList[p_GroupType].end())
        return;

    std::vector<Unit*>::iterator l_It = std::find(m_GroupList[p_GroupType][p_GroupName].begin(), m_GroupList[p_GroupType][p_GroupName].end(), p_Unit);
    if (l_It == m_GroupList[p_GroupType][p_GroupName].end())
        return;

    m_GroupList[p_GroupType][p_GroupName].erase(l_It);

    if (m_GroupList[p_GroupType][p_GroupName].empty())
        m_GroupList[p_GroupType].erase(m_GroupList[p_GroupType].find(p_GroupName));

    if (m_GroupList[p_GroupType].empty())
        m_GroupList.erase(m_GroupList.find(p_GroupType));
}

void GroupManager::RemoveUnitFromAllGroupType(eGroupType p_GroupType, Unit* p_Unit)
{
    ;
}

std::vector< Unit* >* GroupManager::GetUnitForGroup(eGroupType p_GroupType, const std::string & p_GroupName)
{
    if (m_GroupList.find(p_GroupType) == m_GroupList.end())
        return nullptr;

    if (m_GroupList[p_GroupType].find(p_GroupName) == m_GroupList[p_GroupType].end())
        return nullptr;

    return &m_GroupList[p_GroupType][p_GroupName];
}

#include "GroupManager.hpp"
#include "Unit.hpp"

GroupManager::GroupManager()
{
}

GroupManager::~GroupManager()
{
}

bool GroupManager::AddUnitToGroup(eGroupType p_GroupType, const std::string & p_GroupName, Unit* p_Unit)
{
    if (std::find(m_GroupList[p_GroupType][p_GroupName].begin(), m_GroupList[p_GroupType][p_GroupName].end(), p_Unit) != m_GroupList[p_GroupType][p_GroupName].end())
        return false;

    for (Unit* l_Unit : m_GroupList[p_GroupType][p_GroupName])
        l_Unit->UnitEnterInGroup(p_Unit, p_GroupName);

    m_GroupList[p_GroupType][p_GroupName].push_back(p_Unit);

    p_Unit->EnterInGroup(p_GroupName);

    if (std::find(m_UnitList[p_Unit][p_GroupType].begin(), m_UnitList[p_Unit][p_GroupType].end(), p_GroupName) != m_UnitList[p_Unit][p_GroupType].end())
        return false;

    m_UnitList[p_Unit][p_GroupType].push_back(p_GroupName);

    return true;
}

void GroupManager::RemoveUnitFromGroup(eGroupType p_GroupType, const std::string & p_GroupName, Unit* p_Unit)
{
    if (m_GroupList.find(p_GroupType) != m_GroupList.end() && m_GroupList[p_GroupType].find(p_GroupName) != m_GroupList[p_GroupType].end())
    {
        std::vector<Unit*>::iterator l_It = std::find(m_GroupList[p_GroupType][p_GroupName].begin(), m_GroupList[p_GroupType][p_GroupName].end(), p_Unit);
        if (l_It != m_GroupList[p_GroupType][p_GroupName].end())
        {
            m_GroupList[p_GroupType][p_GroupName].erase(l_It);
            p_Unit->LeaveGroup(p_GroupName);
            for (Unit* l_Unit : m_GroupList[p_GroupType][p_GroupName])
                l_Unit->UnitLeaveGroup(p_Unit, p_GroupName);

            if (m_GroupList[p_GroupType][p_GroupName].empty())
                m_GroupList[p_GroupType].erase(m_GroupList[p_GroupType].find(p_GroupName));

            if (m_GroupList[p_GroupType].empty())
                m_GroupList.erase(m_GroupList.find(p_GroupType));
        }
    }

    /// Unit Side
    if (m_UnitList.find(p_Unit) != m_UnitList.end() && m_UnitList[p_Unit].find(p_GroupType) != m_UnitList[p_Unit].end())
    {
        std::vector<std::string>::iterator l_It = std::find(m_UnitList[p_Unit][p_GroupType].begin(), m_UnitList[p_Unit][p_GroupType].end(), p_GroupName);
        if (l_It != m_UnitList[p_Unit][p_GroupType].end())
        {
            m_UnitList[p_Unit][p_GroupType].erase(l_It);

            if (m_UnitList[p_Unit][p_GroupType].empty())
                m_UnitList[p_Unit].erase(m_UnitList[p_Unit].find(p_GroupType));

            if (m_UnitList[p_Unit].empty())
                m_UnitList.erase(m_UnitList.find(p_Unit));
        }
    }
}

void GroupManager::RemoveUnitFromAllGroupType(eGroupType p_GroupType, Unit* p_Unit)
{
    std::vector<std::string> l_ListGroupName;
    /// Unit Side
    if (m_UnitList.find(p_Unit) != m_UnitList.end() && m_UnitList[p_Unit].find(p_GroupType) != m_UnitList[p_Unit].end())
    {
        for (std::vector<std::string>::iterator l_It = m_UnitList[p_Unit][p_GroupType].begin(); l_It != m_UnitList[p_Unit][p_GroupType].end();l_It++)
        {
            l_ListGroupName.push_back(*l_It);
        }
    }
    for (uint16 i = 0; i < l_ListGroupName.size(); i++)
        RemoveUnitFromGroup(p_GroupType, l_ListGroupName[i], p_Unit);
}

void GroupManager::RemoveUnitFromAllGroup(Unit* p_Unit)
{
    std::vector<eGroupType> l_ListGroupType;
    /// Unit Side
    for (std::map<eGroupType, std::vector<std::string> >::iterator l_It = m_UnitList[p_Unit].begin(); l_It != m_UnitList[p_Unit].end();l_It++)
    {
        l_ListGroupType.push_back((*l_It).first);
    }
    for (uint16 i = 0; i < l_ListGroupType.size(); i++)
        RemoveUnitFromAllGroupType(l_ListGroupType[i], p_Unit);
}

std::vector< Unit* >* GroupManager::GetUnitForGroup(eGroupType p_GroupType, const std::string & p_GroupName)
{
    if (m_GroupList.find(p_GroupType) == m_GroupList.end())
        return nullptr;

    if (m_GroupList[p_GroupType].find(p_GroupName) == m_GroupList[p_GroupType].end())
        return nullptr;

    return &m_GroupList[p_GroupType][p_GroupName];
}

uint16 GroupManager::GetNBPlayerForGroup(eGroupType p_GroupType, const std::string & p_GroupName)
{
    if (m_GroupList.find(p_GroupType) == m_GroupList.end())
        return 0;

    if (m_GroupList[p_GroupType].find(p_GroupName) == m_GroupList[p_GroupType].end())
        return 0;

    return m_GroupList[p_GroupType][p_GroupName].size();
}

std::vector<std::string>* GroupManager::GetGroupForUnit(eGroupType p_GroupType, Unit* p_Unit)
{
    if (m_UnitList.find(p_Unit) == m_UnitList.end())
        return nullptr;

    if (m_UnitList[p_Unit].find(p_GroupType) == m_UnitList[p_Unit].end())
        return nullptr;

    return &m_UnitList[p_Unit][p_GroupType];
}

bool GroupManager::UnitsInGroup(Unit* p_Unit1, Unit* p_Unit2)
{
    std::vector<eGroupType> l_ListGroupType;
    /// Unit Side
    if (m_UnitList.find(p_Unit2) == m_UnitList.end())
        return false;

    for (std::map<eGroupType, std::vector<std::string> >::iterator l_It = m_UnitList[p_Unit1].begin(); l_It != m_UnitList[p_Unit1].end(); l_It++)
    {
        if (m_UnitList[p_Unit2].find((*l_It).first) == m_UnitList[p_Unit2].end())
            continue;

        for (std::vector<std::string>::iterator l_Itr = m_UnitList[p_Unit1][(*l_It).first].begin(); l_Itr != m_UnitList[p_Unit1][(*l_It).first].end(); l_Itr++)
        {
            if (std::find(m_UnitList[p_Unit2][(*l_It).first].begin(), m_UnitList[p_Unit2][(*l_It).first].end(), (*l_Itr)) == m_UnitList[p_Unit2][(*l_It).first].end())
                continue;
            else
                return true;
        }
    }
    return false;
}
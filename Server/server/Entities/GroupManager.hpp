#pragma once
#include "../Define.hpp"
#include <map>
#include <vector>

class Unit;
class GroupManager
{
public:
    GroupManager();
    ~GroupManager();
    void AddUnitToGroup(eGroupType, const std::string &, Unit*);
    void RemoveUnitFromGroup(eGroupType, const std::string &, Unit*);
    void RemoveUnitFromAllGroupType(eGroupType, Unit*);
    std::vector< Unit* >* GetUnitForGroup(eGroupType, const std::string &);

private:
    std::map<eGroupType, std::map<std::string, std::vector< Unit* > > > m_GroupList;
};


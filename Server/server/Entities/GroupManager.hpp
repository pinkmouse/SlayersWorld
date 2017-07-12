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
    bool AddUnitToGroup(eGroupType, const std::string &, Unit*);
    void RemoveUnitFromGroup(eGroupType, const std::string &, Unit*);
    void RemoveUnitFromAllGroupType(eGroupType, Unit*);
    void RemoveUnitFromAllGroup(Unit*);
    std::vector< Unit* >* GetUnitForGroup(eGroupType,  const std::string &);
    uint16 GetNBPlayerForGroup(eGroupType, const std::string &);
    std::vector<std::string>* GetGroupForUnit(eGroupType, Unit*);
    bool UnitsInGroup(Unit*, Unit*);

private:
    std::map<eGroupType, std::map<std::string, std::vector< Unit* > > > m_GroupList;
    std::map<Unit*, std::map<eGroupType, std::vector<std::string> > > m_UnitList;
};


#pragma once
#include "../Define.hpp"
#include <map>
#include <vector>

class Player;
class GroupManager
{
public:
    GroupManager();
    ~GroupManager();
    void AddPlayerToGroup(eGroupType, const std::string &, Player*);
    void RemovePlayerFromGroup(eGroupType, const std::string &, Player*);
    void RemovePlayerFromAllGroupType(eGroupType, Player*);
    std::vector< Player* >* GetPlayerForGroup(eGroupType, const std::string &);

private:
    std::map<eGroupType, std::map<std::string, std::vector< Player* > > > m_GroupList;
};


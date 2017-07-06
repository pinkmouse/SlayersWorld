#pragma once
#include "../Define.hpp"
#include <map>

class UnitManager
{
public:
    UnitManager();
    ~UnitManager();
    void AddCreatureTemplate(const CreatureTemplate &);
    CreatureTemplate* GetCreatureTemplate(uint32);
    void AddGossip(const Gossip &);
    std::vector<Gossip>* GetGossipListFor(TypeUnit, uint16);

private:
    std::map<uint16, CreatureTemplate> m_CreatureTemplateMap;
    std::map < TypeUnit, std::map < uint16, std::vector< Gossip > > > m_MapGossip;
};


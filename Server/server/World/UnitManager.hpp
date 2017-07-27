#pragma once
#include "../Define.hpp"
#include <map>

class UnitManager
{
public:
    UnitManager();
    ~UnitManager();
    void AddCreatureTemplate(const CreatureTemplate &);
    void AddAnimationUnitTemplate(const AnimationUnitTemplate &);
    CreatureTemplate* GetCreatureTemplate(uint32);
    AnimationUnitTemplate* GetAnimationUnitTemplate(uint32);
    void AddGossip(const Gossip &);
    std::vector<Gossip>* GetGossipListFor(TypeUnit, uint16);

private:
    std::map<uint16, CreatureTemplate> m_CreatureTemplateMap;
    std::map<uint16, AnimationUnitTemplate> m_AnimationUnitTemplateMap;
    std::map < TypeUnit, std::map < uint16, std::vector< Gossip > > > m_MapGossip;
};


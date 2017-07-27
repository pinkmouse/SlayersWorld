#pragma once
#include "Case.hpp"
#include <string>
#include <vector>
#include <map>
#include "../Entities/DynamicObject.hpp"
#include "../Entities/Player.hpp"
#include "../Entities/Creature.hpp"
#include "../Entities/GameObject.hpp"
#include "../Entities/Areatrigger.hpp"
#include "Square.hpp"

struct BGTemplate
{
    uint16 m_ID;
    uint16 m_MapID;
    uint16 m_MinPlayer;
    uint16 m_MaxPlayer;
    uint16 m_TimeMax;

    BGTemplate() {}

    BGTemplate(uint16 p_ID, uint16 p_MapID, uint16 p_MinPlayer, uint16 p_MaxPlayer, uint16 p_TimeMax) :
        m_ID(p_ID), m_MapID(p_MapID), m_MinPlayer(p_MinPlayer), m_MaxPlayer(p_MaxPlayer), m_TimeMax(p_TimeMax) {}
};

struct GobMapTemplate
{
    uint16 m_ID;
    uint16 m_MapID;
    GameObjectTemplate* m_Template;
    uint32 m_PosX;
    uint32 m_PosY;
    uint32 m_CaseNb;
    std::vector<Gossip>* m_GossipList;

    GobMapTemplate() {}

    GobMapTemplate(uint16 p_ID, uint16 p_MapID, GameObjectTemplate* p_Template, uint32 p_CaseNb, uint32 p_PosX, uint32 p_PosY, std::vector<Gossip>* p_GossipList) :
        m_ID(p_ID), m_MapID(p_MapID), m_Template(p_Template), m_CaseNb(p_CaseNb), m_PosX(p_PosX), m_PosY(p_PosY), m_GossipList(p_GossipList) {}
};

struct AreaTriggerMapTemplate
{
    uint16 m_ID;
    uint16 m_MapID;
    AreatriggerTemplate* m_Template;
    uint32 m_PosX;
    uint32 m_PosY;
    uint32 m_CaseNb;
    std::vector<Gossip>* m_GossipList;

    AreaTriggerMapTemplate() {}

    AreaTriggerMapTemplate(uint16 p_ID, uint16 p_MapID, AreatriggerTemplate* p_Template, uint32 p_CaseNb, uint32 p_PosX, uint32 p_PosY, std::vector<Gossip>* p_GossipList) :
        m_ID(p_ID), m_MapID(p_MapID), m_Template(p_Template), m_CaseNb(p_CaseNb), m_PosX(p_PosX), m_PosY(p_PosY), m_GossipList(p_GossipList) {}
};

struct AnimationUnitMapTemplate
{
    uint16 m_ID;
    uint16 m_MapID;
    AnimationUnitTemplate* m_Template;
    uint32 m_PosX;
    uint32 m_PosY;
    std::vector<Gossip>* m_GossipList;

    AnimationUnitMapTemplate() {}

    AnimationUnitMapTemplate(uint16 p_ID, uint16 p_MapID, AnimationUnitTemplate* p_Template, uint32 p_PosX, uint32 p_PosY, std::vector<Gossip>* p_GossipList) :
        m_ID(p_ID), m_MapID(p_MapID), m_Template(p_Template), m_PosX(p_PosX), m_PosY(p_PosY), m_GossipList(p_GossipList) {}
};


struct CreatureMapTemplate
{
    uint16 m_ID;
    uint16 m_MapID;
    CreatureTemplate* m_Template;
    uint32 m_PosX;
    uint32 m_PosY;
    std::vector<Gossip>* m_GossipList;

    CreatureMapTemplate() {}

    CreatureMapTemplate(uint16 p_ID, uint16 p_MapID, CreatureTemplate* p_Template, uint32 p_PosX, uint32 p_PosY, std::vector<Gossip>* p_GossipList) :
        m_ID(p_ID), m_MapID(p_MapID), m_Template(p_Template), m_PosX(p_PosX), m_PosY(p_PosY), m_GossipList(p_GossipList) {}
};

class MapTemplate
{
public:
    MapTemplate();
    MapTemplate(uint16, eTypeMap, uint16, const std::string &, const std::string &, const std::string &, bool);
	~MapTemplate();

    bool InitializeMap();
    void AddZone(Zone);
    void AddGobMapTemplate(GobMapTemplate);
    void AddCreatureMapTemplate(CreatureMapTemplate);
    void AddAnimationUnitMapTemplate(AnimationUnitMapTemplate);
    void AddAreaTriggerMapTemplate(AreaTriggerMapTemplate);
    Case* GetCase(uint32);

    bool IsInstance() const;
    uint16 GetID() const;
    uint16 GetSizeX() const;
    uint16 GetSizeY() const;
    eTypeMap GetType() const;
    std::string GetFileName() const;
    std::string GetFileChipset() const;
    std::string GetName() const;
    uint16 GetMaxPlayer() const;
    std::vector<Case>*	GetListCase();
    std::vector<Zone>*	GetListZone();
    std::vector<GobMapTemplate>*	        GetListGob();
    std::vector<AreaTriggerMapTemplate>*	GetListArea();
    std::vector<CreatureMapTemplate>*	    GetListCreature();
    std::vector<AnimationUnitMapTemplate>*	GetListAnimationUnit();


private:
	uint16 m_ID;
    bool m_Instance;
	uint16 m_SizeX;
	uint16 m_SizeY;
    uint16 m_MaxPlayer;
    std::string m_Name;
    std::string m_FileName;
    std::string m_FileChipset;
    eTypeMap m_TypeMap;

    std::map< eInterMapAction, std::queue<Unit* > > m_UnitInterMapActionQueue;
	std::vector<Case>	m_ListCase;
    std::vector<Zone>	m_ListZone;
    std::vector<GobMapTemplate>	m_ListGoB;
    std::vector<AreaTriggerMapTemplate>	m_ListArea;
    std::vector<CreatureMapTemplate>	m_ListCreature;
    std::vector<AnimationUnitMapTemplate>	m_ListAnimationUnit;

	struct          t_MapParam
	{
		int l_ID;
		int l_Size[2];
	};

	struct          t_Case
	{
		int l_TabTileNb[6];
		bool l_Block;
	};
};

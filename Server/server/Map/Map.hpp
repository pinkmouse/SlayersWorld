#pragma once
#include "../Define.hpp"
#include "Case.hpp"
#include <SFML/System/Time.hpp>
#include <string>
#include <vector>
#include <map>
#include "../Entities/DynamicObject.hpp"
#include "../Entities/Player.hpp"
#include "../Entities/Creature.hpp"
#include "../Entities/AnimationUnit.hpp"
#include "../Entities/GroupManager.hpp"
#include "Square.hpp"
#include "../World/WorldPacket.hpp"
#include "MapTemplate.hpp"

class Map
{
public:
    Map(uint16, MapTemplate*);
	~Map();
    bool Initialize();
	bool InitializeMap(const std::string &);
    virtual void Update(sf::Time);
    virtual void AddUnit(Unit*);
    virtual void RemoveUnit(Unit*);
    uint16  GetSquareID(uint16 , uint16) const;
    Unit* GetUnit(TypeUnit, uint16);
    std::vector<Unit*> GetAllUnitWithEntry(TypeUnit, uint16);
    uint16 GetID() const;
    uint16 ChangeSquare(Unit*);
    Case* GetCase(uint32) const;
    Case* GetCase(uint32, uint32) const;
    int64 GetCaseNb(const int64 &, const int64 &) const;
    void RemoveFromSquare(Unit*);
    void AddToSquare(Unit*, uint16);
    std::vector<Square*> GetSquareSet(uint16);
    Square* GetSquare(uint16);
    std::vector<uint16> GetSquareSetID(uint16);

    Unit* GetCloserUnit(Unit*, float p_Range = 2.0f, bool p_OnlyInLife = false, bool p_InFront = true, bool p_Attackable = false);
    std::vector<Unit*> GetUnitsInRadius(Unit*, float p_RangeMin = 0.0f, float p_RangeMax = 2.0f, bool p_OnlyInLife = false, bool p_Attackable = false, float p_Angle = 360.0f);
    std::vector<Unit*> GetUnitsInRadius(WorldObject*, float p_RangeMin = 0.0f, float p_RangeMax = 2.0f, bool p_OnlyInLife = false, float p_Angle = 360.0f);
    std::vector<Unit*> GetUnitsInCase(uint32, uint32);

    uint16 GetSizeX() const;
    uint16 GetSizeY() const;
    uint16 GetInstanceID() const;
    void AddUnitInterMapAction(eInterMapAction, Unit*);
    std::queue<Unit*>* GetUnitInterMapAction(eInterMapAction);
    void UpdateForPlayersInNewSquare(Unit*, bool p_UpdateAll = false);
    std::map<uint16, Unit*>* GetListUnitType(TypeUnit);
    uint16 GetNbUnitType(TypeUnit);

    void SetListCase(std::vector<Case*>);
    void AddCase(Case*);
    GroupManager* GetGroupManager();
    bool UnitIsInVisu(Unit*, Unit*);
    virtual void UnitUnaura(Unit*, const uint16 &) {};
    virtual void UnitAddaura(Unit*, const uint16 &) {};

    virtual bool LauchTrapHandle(GameObject*, Unit*) { return true; };
    virtual bool IsBattleground() { return false; };

    void SendMsgToMap(const std::string &);
    /// Enable Zone
    void AddZone(Zone*);
    void EnableZone(uint16, bool);

    /// Network
    void SendToSet(WorldPacket, Unit*);
    void SendToMap(WorldPacket);

    /// Pathfinding
    Path LaunchPathFinding(const Position &, const Position &);

    /// For Instance
    virtual bool IsFinish();

protected:
    GroupManager* m_GroupManager;

private:
	uint16 m_ID;
    uint16 m_InstanceID;
	uint16 m_SizeX;
	uint16 m_SizeY;
    MapTemplate* m_MapTemplate;

    std::map< eInterMapAction, std::queue<Unit* > > m_UnitInterMapActionQueue;
	std::vector<Case*>	m_ListCase;
    std::map<uint8, Zone*>	m_ListZone;
    std::map<TypeUnit, std::map<uint16, Unit*> > m_ListUnitZone;

    std::map<uint16, Square> m_ListSquare;

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

    ///*****************************
    /// --------- PATHFINDING*******
    struct PFNode
    {
        uint32 m_CostG; ///< Cost to go from start to Node
        uint32 m_CostH; ///< Cost to go from Node to end
        uint32 m_CostF; ///< Total of Node parents cost
        Position m_PosParent; ///< Postion of parent

        PFNode() :
            m_CostG(0), m_CostH(0), m_CostF(0) 
        {
            m_PosParent.m_X = 0, m_PosParent.m_Y = 0;
        }
    };

    typedef std::map< Position, PFNode > ListNode;
    Position m_StartPosition;
    Position m_EndPosition;
    ListNode m_CloseList;
    ListNode m_OpenList;
    bool AlreadyInList(const Position &, const ListNode &);
    void AddAdjacentCases(const Position &);
    const Position BestNode(ListNode &);
    void AddToCloseList(const Position &);
    Path GetPath();
    ///*******************************
};
#pragma once
#include "../Define.hpp"
#include "../Entities/WorldObject.hpp"
#include "../Entities/Unit.hpp"
#include "../Entities/DynamicObject.hpp"

class Case : public WorldObject
{
public:
	Case(uint32, uint16, uint16);
	~Case();
	void SetBlock(bool);
    bool CanBeWalk();
    bool IsBlocking() const;
    uint32 GetID() const;
    void AddDynamicOject(DynamicObject*);
    void AddZone(Zone*);
    void UnitEnterInCase(Unit*, Case*);
    bool IsInZoneType(eTypeZone);
    bool HasZone(uint16);
    std::vector<Zone*> CompareZones(Case*);

private:
	bool m_Block;
	uint32 m_ID;
    std::vector<Zone*> m_ZoneList;
    std::vector<DynamicObject*> m_DynamicObjectList;
};


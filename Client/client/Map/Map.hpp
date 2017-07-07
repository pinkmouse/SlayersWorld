#pragma once

#include "Case.hpp"
#include "../Define.hpp"
#include <string>
#include <map>
#include <vector>
#include "../Entities/Player.hpp"

class Map
{
public:
	Map(uint16);
	~Map();
	bool InitializeMap(const std::string &, const std::string &);
	bool IsValidMap();
	
	uint16 GetSquareID(uint16, uint16) const;
	std::vector<Case*> GetSquare(uint16);
	std::vector<std::vector<Case*>> GetSquareZone(uint16);
    std::vector<uint16> GetSquareSetID(uint16);
	Case* GetCase(uint16) const;
    Case* GetCase(uint32, uint32) const;
	uint16 GetSizeX() const;
	uint16 GetSizeY() const;
    uint16 GetID() const;
    Unit* GetUnit(TypeUnit, uint16);
    void MoveUnitToDirection(TypeUnit, uint16, Position, uint8);
    std::map<TypeUnit, std::map<uint16, Unit*>>* GetListUnitZone();
    std::string GetChipsetFileName() const;

    void AddUnit(Unit*);
    void RemoveUnit(Unit*);

    void Update(sf::Time);

private:
	uint16 m_ID;
	uint16 m_SizeX;
	uint16 m_SizeY;
    std::string m_ChipsetName;

	std::vector<Case*> m_ListCase;
	std::map<uint16, std::vector<Case*>>	m_MapListCase;

    std::map<TypeUnit, std::map<uint16, Unit*>> m_ListUnitZone;

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


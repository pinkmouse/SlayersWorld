#pragma once
#include "../Define.hpp"
#include "Case.hpp"
#include <SFML/System/Time.hpp>
#include <string>
#include <vector>
#include <map>
#include "../Entities/Unit.hpp"
#include "../Entities/Player.hpp"
#include "Square.hpp"

class Map
{
public:
	Map();
	~Map();
	bool InitializeMap(const std::string &);
    void Update(sf::Time);
    void AddUnit(Unit*);
    void RemoveUnit(Unit*);
    uint16  GetSquareID(uint16 , uint16) const;
    Unit* GetUnit(TypeUnit, uint16);
    uint16 ChangeSquare(Unit*);
    Case* GetCase(uint16) const;
    Case* GetCase(uint32, uint32) const;
    void RemoveFromSquare(Unit*);
    void AddToSquare(Unit*, uint16);
    std::vector<Square*> GetSquareSet(uint16);
    Square* GetSquare(uint16);
    std::vector<uint16> GetSquareSetID(uint16);
    uint16 GetSizeX() const;
    uint16 GetSizeY() const;
    void UpdateForPlayersInNewSquare(Unit*, bool p_UpdateAll = false);

private:
	uint16 m_ID;
	uint16 m_SizeX;
	uint16 m_SizeY;

	std::vector<Case*>	m_ListCase;
    std::map<TypeUnit, std::map<uint16, Unit*>> m_ListUnitZone;

    std::map<uint16, Square> m_ListSquare;

	struct          t_MapParam
	{
		int l_ID;
		int l_Size[2];
	};

	struct          t_Case
	{
		int l_TabTileNb[4];
		bool l_Block;
	};
};


#pragma once
#include "MovementHandler.hpp"
#include "../Define.hpp"
#include <string>

class Map;
class Player;

class Unit
{
public:
    Unit(uint16);
    Unit(uint16, TypeUnit);
    ~Unit();

    std::string GetName() const;
    uint16 GetMapID() const;
    uint32 GetPosX() const;
    uint32 GetPosY() const;
    uint8 GetLevel() const;
    uint8 GetSkinID() const;
    uint8 GetOrientation() const;
    Map* GetMap() const;
    uint16 GetSquareID() const;
    uint16 GetID() const;

    void SetName(const std::string &);
    void SetLevel(const uint8 &);
    void SetMapID(const uint16 &);
    void SetPosX(const uint32 &);
    void SetPoxY(const uint32 &);
    void SetSkinID(const uint8 &);
    void SetOrientation(const Orientation &);
    void SetMap(Map*);
    void SetSquareID(uint16);
    TypeUnit GetType() const;

    Player* ToPlayer();
    MovementHandler* GetMovementHandler();

    void Update(sf::Time);

protected:
    std::string m_Name;
    uint8 m_Level;
    uint16 m_MapID;
    uint32 m_PosX;
    uint32 m_PosY;
    uint8 m_SkinID;
    Orientation m_Orientation;
    MovementHandler* m_MovementHandler;
    TypeUnit m_Type;
    Map* m_Map;
    uint16 m_SquareID;
    uint16 m_ID;
};

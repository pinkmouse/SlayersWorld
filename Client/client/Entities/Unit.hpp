#pragma once
#include "../Define.hpp"
#include "MovementHandler.hpp"
#include <string>

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
    uint8 GetSizeX() const;
    uint8 GetSizeY() const;
    uint8 GetLevel() const;
    uint8 GetSkinID() const;
    uint16 GetID() const;
    Orientation GetOrientation() const;
    TypeUnit GetType() const;
    uint8 GetOpacity() const;

    void SetName(const std::string &);
    void SetLevel(const uint8 &);
    void SetMapID(const uint16 &);
    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetSkinID(const uint8 &);
    void SetOrientation(const Orientation &);
    void SetMap(Map*);

    bool IsInRayVisible(Unit*);

    MovementHandler* GetMovementHandler();
    Player* ToPlayer();

    void Update(sf::Time);

protected:
    std::string m_Name;
    uint16 m_ID;
    uint8 m_Level;
    uint8 m_SkinID;
    uint16 m_MapID;

    uint32 m_PosX;
    uint32 m_PosY;

    uint8 m_SizeX;
    uint8 m_SizeY;

    Orientation m_Orientation;
    MovementHandler* m_MovementHandler;
    TypeUnit m_Type;
    uint8 m_Opacity;
    uint64 m_DiffTimeOpactiy;

    Map* m_Map;
};


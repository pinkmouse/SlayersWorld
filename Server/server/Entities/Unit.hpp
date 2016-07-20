#pragma once
#include "../Define.hpp"
#include "../Map/Map.hpp"
#include <string>

class Unit
{
public:
    Unit();
    ~Unit();

    std::string GetName() const;
    uint16 GetMapID() const;
    uint32 GetPosX() const;
    uint32 GetPosY() const;
    uint8 GetLevel() const;
    uint8 GetSkinID() const;
    uint8 GetOrientation() const;
    Map* GetMap() const;

    void SetName(const std::string &);
    void SetLevel(const uint8 &);
    void SetMapID(const uint16 &);
    void SetPosX(const uint32 &);
    void SetPoxY(const uint32 &);
    void SetSkinID(const uint8 &);
    void SetOrientation(const uint8 &);
    void SetMap(Map*);

protected:
    std::string m_Name;
    uint8 m_Level;
    uint16 m_MapID;
    uint32 m_PosX;
    uint32 m_PosY;
    uint8 m_SkinID;
    uint8 m_Orientation;

    Map* m_Map;
};


#pragma once
#include "../Define.hpp"
#include "../System/Resource/Resource.hpp"
#include "WorldObject.hpp"
#include "MovementHandler.hpp"
#include <map>
#include <string>

class Player;

class Unit : public WorldObject
{
public:
    Unit(uint16);
    Unit(uint16, TypeUnit);
    ~Unit();

    bool IsPlayer() const;
    std::string GetName() const;
    uint16 GetCasePosX() const;
    uint16 GetCasePosY() const;
    uint8 GetLevel() const;
    uint8 GetSkinID() const;
    uint16 GetID() const;
    float GetSkinZoomFactor() const;
    Orientation GetOrientation() const;
    TypeUnit GetType() const;
    uint8 GetOpacity();
    bool IsDeath();

    void SetName(const std::string &);
    void SetLevel(const uint8 &);
    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetSkinID(const uint8 &);
    void SetOrientation(const Orientation &);
    void SetSkinZoomFactor(const float &);
    void SetMap(Map*);
    
    void StartMovement();
    bool IsInRayVisible(Unit*);
    void TeleportTo(const WorldPosition&);

    MovementHandler* GetMovementHandler();
    Player* ToPlayer();

    Resource* GetResource(eResourceType);
    uint8 GetResourceNb(eResourceType);
    void SetResourceNb(eResourceType, uint8);

    void SetTalk(const std::string &);
    std::string GetTalk() const;
	void AddDamageLog(const DamageInfo &);
	std::vector<std::pair<DamageInfo, uint32>>& GetDamageLog();
    void Update(sf::Time);

protected:
    std::string m_Name;
    uint16 m_ID;
    uint8 m_Level;
    uint8 m_SkinID;
    float m_SkinZoomFactor;
    std::map< eResourceType, Resource* > m_Resources;

    MovementHandler* m_MovementHandler;
    TypeUnit m_Type;
    uint8 m_Opacity;
    uint64 m_DiffTimeOpactiy;
	std::vector<std::pair<DamageInfo, uint32>> m_HistoryDamage;

    Map* m_Map;

    std::string m_Talk;
    uint64 m_DiffTimeTalk;
};


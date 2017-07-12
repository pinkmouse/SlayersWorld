#pragma once
#include "../Define.hpp"
#include "../System/Resource/Resource.hpp"
#include "WorldObject.hpp"
#include "MovementHandler.hpp"
#include "../Graphics/VisualEffect.hpp"
#include <map>
#include <string>

class Player;
class DynamicObject;

class Unit : public WorldObject
{
public:
    Unit(uint16);
    Unit(uint16, TypeUnit, uint8, uint8);
    virtual ~Unit();

    bool IsPlayer() const;
    bool IsDynamicObject() const;
    std::string GetName() const;
    uint16 GetCasePosX() const;
    uint16 GetCasePosY() const;
    uint8 GetLevel() const;
    int16 GetSkinID() const;
    uint16 GetID() const;
    float GetSkinZoomFactor() const;
    float GetSpeed() const;
    Orientation GetOrientation() const;
    TypeUnit GetType() const;
    bool GetIsInGroup() const;
    bool IsDeath();
    float GetPosXAtIntant(); /// Use for drawing
    float GetPosYAtIntant(); /// Use for drawing

    void SetName(const std::string &);
    void SetLevel(const uint8 &);
    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetSkinID(const int16 &);
    void SetOrientation(const Orientation &);
    void SetSkinZoomFactor(const float &);
    void SetMap(Map*);
    void SetSpeed(float);
    void SetIsInGroup(bool);
    
    void StartMovement();
    bool IsInRayVisible(Unit*);
    void TeleportTo(const WorldPosition&);

    MovementHandler* GetMovementHandler();
    Player* ToPlayer();
    DynamicObject* ToDynamicObject();

    Resource* GetResource(eResourceType);
    uint8 GetResourceNb(eResourceType);
    void SetResourceNb(eResourceType, uint8);

    void SetTalk(const std::string &);
    std::string GetTalk() const;
	void AddDamageLog(const DamageInfo &);
	std::vector<std::pair<DamageInfo, uint32>> GetDamageLog();

    void AddVisualEffect(VisualEffect);
    std::vector< VisualEffect >  *GetVisualsEffect();
    void LaunchCastBar(uint16);
    uint8 GetCastPct();
    void CleanCastBar();

    void Update(sf::Time);
    void UpdateOpacity();

protected:
    std::string m_Name;
    uint16 m_ID;
    uint8 m_Level;
    int16 m_SkinID;
    float m_SkinZoomFactor;
    bool m_IsInGroup;
    std::map< eResourceType, Resource* > m_Resources;

    MovementHandler* m_MovementHandler;
    TypeUnit m_Type;
    uint64 m_DiffTimeOpactiy;
	std::vector< std::pair< DamageInfo, uint32 > > m_HistoryDamage;
    std::vector< VisualEffect > m_VisualEffects;
    std::pair< uint64, uint64> m_CastTime;
    Map* m_Map;

    std::string m_Talk;
    uint64 m_DiffTimeTalk;
};


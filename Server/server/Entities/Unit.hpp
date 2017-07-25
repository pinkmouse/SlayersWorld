#pragma once
#include "WorldObject.hpp"
#include "MovementHandler.hpp"
#include "MovementHandlerCreature.hpp"
#include "../Define.hpp"
#include "../System/Resource/Resource.hpp"
#include "../System/Spell/SpellTemplate.hpp"
#include <string>
#include <map>

/* DOUBLE INCLUSION */
class Map;
class Spell;
class Aura;
class AuraEffect;
class Player;
class Creature;
class GameObject;
class DynamicObject;

class Unit : public WorldObject
{
protected:
    Unit(uint16);
    Unit(uint16, TypeUnit, eFactionType);

public:
    ~Unit();

    /* BASIC */
    bool IsPlayer() const;
    bool IsCreature() const;
    std::string GetName() const;
    uint8 GetSizeX() const;
    uint8 GetSizeY() const;
    uint8 GetLevel() const;
    int16 GetSkinID() const;
    uint16 GetID() const;
    float GetSpeed() const;
    uint8 GetSpeedUint8() const;
    bool IsDeath();
    void SetName(const std::string &);
    virtual void SetLevel(const uint8 &);
    bool IsInWorld() const;
    void SetInWorld(bool);
    void SetSpeed(float);
    TypeUnit GetType() const;
    Creature* ToCreature();
    Player* ToPlayer();
    GameObject* ToGameObject();
    DynamicObject* ToDynamicObject();
    MovementHandler* GetMovementHandler();
    eFactionType GetFaction() const;
    virtual void SetPointsSet(const PointsSet &);
    PointsSet GetPointsSet() const;
    virtual bool AddPointsStat(eStats, uint8);
    virtual bool SubPointsStat(eStats, uint8);
    virtual void ActionFrom(Player*);
    virtual void Respawn();
    virtual void Unspawn();
    virtual bool IsBlocking() const;
    bool IsCenteredInCase();
    Orientation OrientationToBeCenteredInCase(const Position &);
    Position GetPositionCentered();
    void Kill();

    /* RESOURCE */
    Resource* GetResource(eResourceType);
    uint8 GetResourceNb(eResourceType);
    virtual void SetResourceNb(eResourceType, uint8);
    virtual void AddResourceNb(eResourceType, uint8);
    void RegenerateAll();

    /* POSITION */
    uint8 GetOrientation() const;
    Map* GetMap() const;
    uint16 GetSquareID() const;
    WorldPosition GetRespawnPoint() const;
    void SetSquareID(uint16);

    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetPos(const uint32 &, const uint32 &);
    void SetSkinID(const int16 &);
    void Talk(const std::string &);
    void SetOrientation(const Orientation &);
    void SetMap(Map*);
    void SetRespawnPosition(const WorldPosition &);
    bool IsInFront(const Position &) const;
    bool IsInFront(Unit const*) const;

    /* COMBAT */
    void AutoAttack(Unit*);
    DamageInfo CalculSpellDamageToTarget(Unit*, const int32 &, const uint8 &);
    DamageInfo CalculHealToTarget(Unit*, const int32 &, const uint8 &);
    void DealDamage(Unit*, DamageInfo);
    void DealHeal(Unit*, DamageInfo);
    bool IsInMovement() const;
    virtual void OutOfCombat();
    bool IsInCombat() const;
    void EnterInEvade();
    void OutOfEvade();
    bool IsInEvade() const;
    void EnterInCombat(Unit*);
    void InCombat();
    void AddAttacker(Unit*);
    void AddVictim(Unit*);
    void RemoveAttacker(Unit*);
    void RemoveVictim(Unit*);
    uint8 GetNbAttacker() const;
    void SetVictim(Unit*);
    Unit* GetMaxThreatAttacker();
    void AddThreadFromAttacker(Unit*, uint16);
    Unit* GetVictim() const;
    bool CanAttack(Unit*);
    bool IsHostileTo(Unit*);
    bool IsFriendlyTo(Unit*);
    bool IsAttackableTarget() const;

    /* UPDATE */
    virtual void Update(sf::Time);
    void UpdateCombat(sf::Time);
    void UpdateDeathState(sf::Time);
    void UpdateRegen(sf::Time);
    void UpdateSpell(sf::Time);
    void UpdateCooldowns(sf::Time);
    void UpdateAura(sf::Time);
    void UpdateVictims();
    void UpdateSpeed();

    /* TELEPORT */
    void TeleportTo(const WorldPosition&);
    void TeleportTo(uint32, uint32, Orientation);
    void TeleportTo(uint16, uint16, uint32, uint32, Orientation);

    /* SPELL */
    void AddSpellID(uint16, uint64);
    void RemoveSpellID(uint16);
    bool HasSpell(uint16) const;
    virtual void CastSpell(uint16);
    void CastSpell(uint16, std::vector<Unit*>);
    std::map< uint16, uint64 >* GetSpellList();
    virtual void AddSpellCooldown(uint16, uint64);
    bool HasSpellCooldown(uint16);
    void SetCurrentSpell(Spell*);
    Spell* GetCurrentSpell() const;
    void InterruptCast();
    void Dismount();

    /* GOSSIP*/
    void SetGossipList(std::vector<Gossip>*);
    Gossip* GetGossip(const uint16 &);
    void GossipTo(Player *);
    void GossipTo(Player *, const uint16 &);
    void UpdateGossip(sf::Time);

    /* AURA */
    void AddAura(Aura*);
    void RemoveAllAura();
    void RemoveAura(Aura*);
    std::vector<AuraEffect*> GetAuraEffectType(eTypeAuraEffect);
    std::vector<Aura*> GetAura(uint16);
    std::vector<Aura*>* GetAuraList();
    Aura* GetCasterAura(uint16, const Unit*);
    AuraEffect* ApplyAuraEffect(uint8, SpellTemplate*, Unit*, eTypeAuraEffect, const int32 &, const int32 &, const int32 &);
    int32 TotalAmountOfAuraType(eTypeAuraEffect);

    /* GROUP */
    virtual void LeaveGroup(const std::string &);
    virtual void EnterInGroup(const std::string &);
    virtual void UnitLeaveGroup(Unit*, const std::string &);
    virtual void UnitEnterInGroup(Unit*,  const std::string &);
    bool IsInGroupWith(Unit*);
    bool IsInSetWith(Unit*);

    /* ZONE */
    void CheckEnterInZone(uint32, uint32, uint32, uint32);
    void CheckOutOfZone(uint32, uint32, uint32, uint32);
    virtual void EnterInZone(Zone*);
    virtual void OutOfZone(Zone*);

protected:
    /* BASIC*/
    uint16 m_ID;
    std::string m_Name;
    uint8 m_Level;
    TypeUnit m_Type;
    uint8 m_SizeX;
    uint8 m_SizeY;
    int16 m_SkinID;
    bool m_InWorld;
    MovementHandler* m_MovementHandler;
    float m_Speed;
    PointsSet m_PointsSet;

    /* RESOURCE */
    std::map< eResourceType, Resource* > m_Resources;

    /* POSITION */
    Map* m_Map;
    uint16 m_MapID;
    uint16 m_SquareID;
    WorldPosition m_RespawnPosition;

    /* COMBAT */
    bool m_Evade;
    uint64 m_ResTimer;
    int64 m_RespawnTime;

private:
    /* COMBAT */
    void CleanAttackers();
    void CleanVictims();
    bool m_InCombat;
    uint64 m_CombatTimer;
    Unit* m_Victim;
    std::map<Unit*, uint16> m_Attackers;
    std::map<Unit*, uint16> m_Victims;

    eFactionType m_FactionType;

    /* AURA */
    std::vector<Aura*> m_AuraList;

    /* SPELL */
    std::map< uint16, uint64 > m_ListSpellID;
    Spell* m_CurrentSpell; ///< Current spell if casting

    /* GOSSIP */
    std::map< eGossipType, std::vector<Gossip> > m_ListGossip;
};

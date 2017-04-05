#pragma once
#include "WorldObject.hpp"
#include "MovementHandler.hpp"
#include "MovementHandlerCreature.hpp"
#include "../Define.hpp"
#include "../System/Resource/Resource.hpp"
#include <string>
#include <map>

/* DOUBLE INCLUSION */
class Map;
class Spell;
class Player;
class Creature;

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
    void SetSquareID(uint16);

    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetSkinID(const int16 &);
    void Talk(const std::string &);
    void SetOrientation(const Orientation &);
    void SetMap(Map*);
    void SetRespawnPosition(const WorldPosition &);
    bool IsInFront(const Position &) const;
    bool IsInFront(Unit const*) const;

    /* COMBAT */
    void AutoAttack(Unit*);
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
    bool CanAttack(Unit*) const;
    bool IsHostileTo(const Unit*) const;
    bool IsFriendlyTo(const Unit*) const;
    bool IsAttackableTarget() const;

    /* UPDATE */
    virtual void Update(sf::Time);
    void UpdateCombat(sf::Time);
    void UpdateDeathState(sf::Time);
    void UpdateRegen(sf::Time);
    void UpdateSpell(sf::Time);
    void UpdateCooldowns(sf::Time);
    void UpdateVictims();

    /* TELEPORT */
    void TeleportTo(const WorldPosition&);
    void TeleportTo(uint32, uint32, Orientation);
    void TeleportTo(uint16, uint32, uint32, Orientation);

    /* SPELL */
    void AddSpellID(uint16, uint64);
    void CastSpell(uint16);
    std::map< uint16, uint64 >* GetSpellList();
    virtual void AddSpellCooldown(uint16, uint64);
    bool HasSpellCooldown(uint16);
    void SetCurrentSpell(Spell*);
    Spell* GetCurrentSpell() const;
    void InterruptCast();

    /* GOSSIP*/
    void SetGossipList(std::vector<Gossip>*);
    void GossipTo(Player *);
    void UpdateGossip(sf::Time);

    /* GROUP */
    bool EnterInGroup(eGroupType, const std::string &);
    void LeaveGroup(eGroupType, const std::string &);
    std::vector< std::string >* GetAllGroupsForType(eGroupType);
    std::map<eGroupType, std::vector< std::string > >* GetAllGroups();
    void LeaveAllGroups();
    void LeaveGroupsType(eGroupType);
    bool IsInGroupWith(const Unit*) const;
    bool IsInGroup(eGroupType, const std::string &) const;

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

    /* SPELL */
    std::map< uint16, uint64 > m_ListSpellID;
    Spell* m_CurrentSpell; ///< Current spell if casting

    /* GOSSIP */
    std::map< eGossipType, std::vector<Gossip> > m_ListGossip;

    /* GROUP*/
    std::map<eGroupType, std::vector< std::string > > m_GroupList;
};

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
    uint8 GetSkinID() const;
    uint16 GetID() const;
    bool IsDeath();
    void SetName(const std::string &);
    void SetLevel(const uint8 &);
    virtual void Respawn();
    bool IsInWorld() const;
    void SetInWorld(bool);
    TypeUnit GetType() const;
    Creature* ToCreature();
    Player* ToPlayer();
    MovementHandler* GetMovementHandler();
    eFactionType GetFaction() const;

    /* RESOURCE */
    Resource* GetResource(eResourceType);
    uint8 GetResourceNb(eResourceType);
    virtual void SetResourceNb(eResourceType, uint8);
    virtual void AddResourceNb(eResourceType, uint8);
    void RegenerateAll();

    /* PATHFINDING */
    PointsSet GetPointsSet() const;
    void SetPointsSet(const PointsSet &);

    /* POSITION */
    uint8 GetOrientation() const;
    Map* GetMap() const;
    uint16 GetSquareID() const;
    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetSkinID(const uint8 &);
    void Talk(const std::string &);
    void SetOrientation(const Orientation &);
    void SetMap(Map*);
    void SetSquareID(uint16);
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
    bool IsHostileTo(Unit*);

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
    void TeleportTo(uint32, uint32);
    void TeleportTo(uint16, uint32, uint32);

    /* SPELL */
    void AddSpellID(uint16, uint64);
    void CastSpell(uint16);
    std::map< uint16, uint64 >* GetSpellList();
    virtual void AddSpellCooldown(uint16, uint64);
    bool HasSpellCooldown(uint16);
    virtual void SetCurrentSpell(Spell*);
    Spell* GetCurrentSpell() const;

    /* GOSSIP*/
    void SetGossipList(std::vector<Gossip>*);
    void GossipTo(Player *);
    void UpdateGossip(sf::Time);

protected:
    /* BASIC*/
    uint16 m_ID;
    std::string m_Name;
    uint8 m_Level;
    TypeUnit m_Type;
    uint8 m_SizeX;
    uint8 m_SizeY;
    uint8 m_SkinID;
    bool m_InWorld;
    MovementHandler* m_MovementHandler;

    /* RESOURCE */
    std::map< eResourceType, Resource* > m_Resources;

    /* POSITION */
    Map* m_Map;
    uint16 m_MapID;
    uint16 m_SquareID;
    WorldPosition m_RespawnPosition;
	PointsSet m_PointsSet;

    /* COMBAT */
    bool m_Evade;
    uint64 m_ResTimer;
    uint64 m_RespawnTime;

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
    std::map< eGossipType, std::vector<Gossip*> > m_ListGossip;
};

#pragma once
#include "WorldObject.hpp"
#include "MovementHandler.hpp"
#include "MovementHandlerCreature.hpp"
#include "../Define.hpp"
#include <string>
#include <map>

class Map;
class Player;
class Creature;

class Unit : public WorldObject
{
protected:
    Unit(uint16);
    Unit(uint16, TypeUnit, eFactionType);

public:
    ~Unit();

    bool IsPlayer() const;
    bool IsCreature() const;
    std::string GetName() const;
    uint8 GetSizeX() const;
    uint8 GetSizeY() const;
    uint8 GetLevel() const;
    uint8 GetSkinID() const;
    uint8 GetOrientation() const;
    Map* GetMap() const;
    uint16 GetSquareID() const;
    uint16 GetID() const;
    uint8 GetHealth() const;
    bool IsDeath() const;
	PointsSet GetPointsSet() const;


    void SetName(const std::string &);
    void SetLevel(const uint8 &);
    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetSkinID(const uint8 &);
    void Talk(const std::string &);
    void SetHealth(const uint8 &);
    void SetOrientation(const Orientation &);
    void SetMap(Map*);
    void SetSquareID(uint16);
    void SetRespawnPosition(const WorldPosition &);
	void SetPointsSet(const PointsSet &);
    bool IsInFront(const Position &) const;
    bool IsInFront(Unit const*) const;
    void DealDamage(Unit*);
    void Respawn();
    bool IsInWorld() const;
    void SetInWorld(bool);
    TypeUnit GetType() const;
    bool IsInMovement() const;

    Creature* ToCreature();
    Player* ToPlayer();
    MovementHandler* GetMovementHandler();

    virtual void Update(sf::Time);
    void UpdateCombat(sf::Time);
    void UpdateDeathState(sf::Time);
    void UpdateRegen(sf::Time);

    bool IsInCombat() const;
    void EnterInEvade();
    void OutOfEvade();
    bool IsInEvade() const;
    void EnterInCombat(Unit*);
    void InCombat();
    void OutOfCombat();
    void SetAttacker(Unit*);
    void SetVictim(Unit*);
    void TeleportTo(const WorldPosition&);
    Unit* GetAttacker() const;
    Unit* GetVictim() const;
    bool CanAttack(Unit*) const;
    bool IsHostileTo(Unit*);
    eFactionType GetFaction() const;

    /// GOSSIP
    void SetGossipList(std::vector<Gossip>*);
    void GossipTo(Player *);
    void UpdateGossip(sf::Time);

protected:
    std::string m_Name;
    uint8 m_Level;
    uint16 m_MapID;
    bool m_InWorld;

    uint8 m_SizeX;
    uint8 m_SizeY;

    uint8 m_Health;
    uint8 m_SkinID;
    MovementHandler* m_MovementHandler;
    TypeUnit m_Type;
    Map* m_Map;
    uint16 m_SquareID;
    uint16 m_ID;

    WorldPosition m_RespawnPosition;
	PointsSet m_PointsSet;
    uint64 m_RespawnTime;
    uint64 m_ResTimer;
    uint64 m_RegenTimer;
    bool m_Evade;

private:
    bool m_InCombat;
    uint64 m_CombatTimer;
    Unit* m_Victim;
    Unit* m_Attacker;
    eFactionType m_FactionType;

    std::map< eGossipType, std::vector<Gossip*> > m_ListGossip;
};

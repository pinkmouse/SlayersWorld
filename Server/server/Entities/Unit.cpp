#include "Unit.hpp"
#include "../Map/Map.hpp"
#include "../World/WorldSocket.hpp"
#include "../World/PacketDefine.hpp"
#include "../System/Resource/ResourceHealth.hpp"
#include <cstdlib>


Unit::Unit(uint16 p_ID)
{
    Unit(p_ID, TypeUnit::CREATURE, eFactionType::Neutral);
}

Unit::Unit(uint16 p_ID, TypeUnit p_Type, eFactionType p_FactionType)
{
    m_Type = p_Type;
    m_FactionType = p_FactionType;
    m_Name = "";
    m_MapID = 0;
    m_ID = p_ID;
    m_SizeX = 24;
    m_SizeY = 32;
    m_Resources[eResourceType::Health] = new ResourceHealth();

    m_InWorld = false;
    m_InCombat = false;
    m_Evade = false;

    m_CombatTimer = 0;
    m_ResTimer = 0;
    m_RespawnTime = 0;
    m_RegenTimer = 0;
    
    m_Victim = nullptr;
    if (m_Type == TypeUnit::CREATURE)
        m_MovementHandler = new MovementHandlerCreature(this, m_SizeX, m_SizeY);
    else
        m_MovementHandler = new MovementHandler(m_SizeX, m_SizeY);
}

Player* Unit::ToPlayer()
{
    if (m_Type == TypeUnit::PLAYER)
        return  reinterpret_cast<Player*>(this);
    else
        return nullptr;
}

Creature* Unit::ToCreature()
{
    if (m_Type == TypeUnit::CREATURE)
        return  reinterpret_cast<Creature*>(this);
    else
        return nullptr;
}

Unit::~Unit()
{
    m_InWorld = false;
    CleanAttackers();
    CleanVictims();
    m_MovementHandler->StopMovement();
    m_MovementHandler->StopAttack();
    m_Map->RemoveUnit(this);
    delete m_MovementHandler;
    m_MovementHandler = nullptr;
}

void Unit::UpdateDeathState(sf::Time p_Diff)
{
    if (IsDeath())
    {
        m_ResTimer += p_Diff.asMicroseconds();
        if (m_ResTimer >= m_RespawnTime)
        {
            Respawn();
            /*switch (m_Type)
            {
            case TypeUnit::PLAYER:
                ToPlayer()->Respawn();
                break;
            case TypeUnit::CREATURE:
                ToCreature()->Respawn();
                break;
            default:
                break;
            }*/
        }
    }
}

void Unit::UpdateCombat(sf::Time p_Diff)
{
    if (!IsInWorld())
        return;

    if (IsInCombat())
    {
        m_CombatTimer += p_Diff.asMicroseconds();

        if (m_CombatTimer >= OUT_OF_COMBAT_TIME * IN_MICROSECOND)
            OutOfCombat();
    }
}

void Unit::UpdateGossip(sf::Time p_Diff)
{
    if (!IsInWorld())
        return;

    if (IsDeath())
        return;

    for (Gossip *l_Gossip : m_ListGossip[eGossipType::Yell])
    {
        l_Gossip->m_GossipTimer += p_Diff.asMicroseconds();
        if (l_Gossip->m_GossipTimer >= l_Gossip->m_Data1 * IN_MICROSECOND)
        {
            Talk(l_Gossip->m_Msg);
            l_Gossip->m_GossipTimer -= l_Gossip->m_Data1 * IN_MICROSECOND;
        }
    }
}


void Unit::UpdateRegen(sf::Time p_Diff)
{
    if (!IsInWorld())
        return;

    if (IsDeath())
        return;

    for (auto l_Resource : m_Resources)
    {
        uint8 l_Before = l_Resource.second->GetNumber();
        l_Resource.second->Update(p_Diff);
        if (l_Before != l_Resource.second->GetNumber()) ///> Hacky to send for player when resource change
            SetResourceNb(l_Resource.first, l_Resource.second->GetNumber());
    }
}

void Unit::Update(sf::Time p_Diff)
{
    UpdateDeathState(p_Diff);
    UpdateCombat(p_Diff);
    UpdateRegen(p_Diff);
    UpdateGossip(p_Diff);
    UpdateVictims();

    if (m_MovementHandler == nullptr)
        return;

    if (!IsInWorld())
        return;

    m_MovementHandler->Update(p_Diff);
    SetPosX(m_MovementHandler->GetPosX());
    SetPosY(m_MovementHandler->GetPosY());

    if (m_MovementHandler->IsDamageReady())
    {
        Unit* l_Unit = m_Map->GetCloserUnit(this, MELEE_RANGE, true, true ,true);

        if (l_Unit != nullptr)
            DealDamage(l_Unit);

        m_MovementHandler->SetDamageDone(true);
    }
}

bool Unit::IsInFront(const Position & p_Position) const
{
    switch (GetOrientation())
    {
        case Orientation::Down:
            if (p_Position.m_Y >= GetPosY())
                return true;
            break;
        case Orientation::Left:
            if (p_Position.m_X <= GetPosX())
                return true;
            break;
        case Orientation::Right:
            if (p_Position.m_X >= GetPosX())
                return true;
            break;
        case Orientation::Up:
            if (p_Position.m_Y <= GetPosY())
                return true;
            break;
        default:
            break;
    }
    return false;
}

void Unit::DealDamage(Unit* p_Victim)
{
	/// DAMAGE
	uint16 l_ForceAttacker = m_PointsSet.m_Force;
	uint16 l_StaminaDefenser = p_Victim->GetPointsSet().m_Stamina;
	int16 l_Balance = l_ForceAttacker - l_StaminaDefenser;
    int8 l_Damage = rand() %  2 + 10;
	l_Damage += (l_Balance * 2);
	l_Damage = std::max(l_Damage, (int8)0);

	/// MISS CHANCE
	uint16 l_DexterityAttacker = m_PointsSet.m_Dexterity;
	uint16 l_DexterityDefenser = p_Victim->GetPointsSet().m_Dexterity;
	l_Balance = l_DexterityDefenser - l_DexterityAttacker;
	int8 l_MissChance = 20 + (l_Balance * 4);
	l_MissChance = std::max(l_MissChance, (int8)0);
	l_MissChance = std::min(l_MissChance, (int8)100);
	bool l_Miss = (rand() % 100) <= l_MissChance;
	if (l_Miss)
		l_Damage = 0;

	int8 l_NewHealth = std::max(p_Victim->GetResourceNb(eResourceType::Health) - l_Damage, 0);

	if (IsPlayer())
		ToPlayer()->GetSession()->SendLogDamage(p_Victim->GetType(), p_Victim->GetID(), l_Damage, l_Miss);

    p_Victim->SetResourceNb(eResourceType::Health, (uint8)l_NewHealth);
    switch (p_Victim->GetType())
    {
        case TypeUnit::PLAYER:
			//p_Victim->ToPlayer()->SetHealth((uint8)l_NewHealth);
			p_Victim->ToPlayer()->GetSession()->SendLogDamage(p_Victim->GetType(), p_Victim->GetID(), l_Damage, l_Miss);
        break;
        /*case TypeUnit::CREATURE:
            p_Victim->ToCreature()->SetHealth((uint8)l_NewHealth);
            break;*/
        default:
            break;
    }

    if (p_Victim->IsDeath())
    {
        if (IsPlayer() && p_Victim->IsCreature())
        {
            ToPlayer()->SetXp(ToPlayer()->GetXp() + p_Victim->ToCreature()->GetXpEarn());
        }
        return;
    }

    if (GetVictim() != p_Victim)
    {
        EnterInCombat(p_Victim);
        p_Victim->AddThreadFromAttacker(this, l_Damage);
    }
    else
    {
        InCombat();
        p_Victim->InCombat();
        p_Victim->AddThreadFromAttacker(this, l_Damage);
    }
}

bool Unit::IsInFront(Unit const* p_Unit) const
{
    return IsInFront(p_Unit->GetPosition());
}

bool Unit::IsInWorld() const
{
    return m_InWorld;
}

void Unit::SetInWorld(bool p_InWorld)
{
    m_InWorld = p_InWorld;
    if (!p_InWorld)
    {
        CleanAttackers();
        CleanVictims();
    }
}

TypeUnit Unit::GetType() const
{
    return m_Type;
}

MovementHandler* Unit::GetMovementHandler()
{
    return m_MovementHandler;
}

std::string Unit::GetName() const
{
    return m_Name;
}

uint8 Unit::GetSizeX() const
{
    return m_SizeX;
}

uint8 Unit::GetSizeY() const
{
    return m_SizeY;
}

uint8 Unit::GetLevel() const
{
    return m_Level;
}

Resource *Unit::GetResource(eResourceType p_Resource)
{
    return m_Resources[p_Resource];
}

uint8 Unit::GetResourceNb(eResourceType p_Resource)
{
    return m_Resources[p_Resource]->GetNumber();
}

void Unit::SetResourceNb(eResourceType p_Resource, uint8 p_Nb)
{
    m_Resources[p_Resource]->SetNumber(p_Nb);

    switch(p_Resource)
    {
        case eResourceType::Health:
            if (!m_Resources[p_Resource]->GetNumber())
                OutOfCombat();
            break;
        default:
            break;
    }
}

uint8 Unit::GetSkinID() const
{
    return m_SkinID;
}

eFactionType Unit::GetFaction() const
{
    return m_FactionType;
}


PointsSet Unit::GetPointsSet() const
{
	return m_PointsSet;
}

bool Unit::IsPlayer() const
{
    return m_Type == TypeUnit::PLAYER;
}

bool Unit::IsCreature() const
{
    return m_Type == TypeUnit::CREATURE;
}

bool Unit::IsInMovement() const
{
    if (m_MovementHandler == nullptr)
        return false;

    return m_MovementHandler->IsInMovement();
}


uint8 Unit::GetOrientation() const
{
    if (m_MovementHandler == nullptr)
        return Orientation::Down;

    return m_MovementHandler->GetOrientation();
}

Map* Unit::GetMap() const
{
    return m_Map;
}

uint16 Unit::GetID() const
{
    return m_ID;
}

void Unit::SetName(const std::string & p_Name)
{
    m_Name = p_Name;
}

void Unit::SetLevel(const uint8 & p_Level)
{
    m_Level = p_Level;
}

void Unit::SetSkinID(const uint8 & p_SkinID)
{
    m_SkinID = p_SkinID;

    PacketUpdateSkin l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(), p_SkinID);
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Unit::Talk(const std::string & p_Talk)
{
    PacketTalk l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(), p_Talk);
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Unit::SetPosX(const uint32 & p_PosX)
{
    WorldObject::SetPosX(p_PosX);
    m_MovementHandler->SetPosX(p_PosX);
}

void Unit::SetPosY(const uint32 & p_PosY)
{
    WorldObject::SetPosY(p_PosY);
    m_MovementHandler->SetPosY(p_PosY);
}

void Unit::SetOrientation(const Orientation & p_Orientation)
{
    m_MovementHandler->SetOrientation(p_Orientation);
}

void Unit::SetMap(Map* p_Map)
{
    m_Map = p_Map;
    m_MovementHandler->SetMap(m_Map);
}

bool Unit::IsDeath()
{
    if (m_Resources[eResourceType::Health]->GetNumber())
        return false;
    return true;
}

uint16 Unit::GetSquareID() const
{
    return m_SquareID;
}

void Unit::SetSquareID(uint16 p_SquareID)
{
    m_SquareID = p_SquareID;
}

void Unit::SetRespawnPosition(const WorldPosition & p_RespawnPosition)
{
    m_RespawnPosition = p_RespawnPosition;
}

void Unit::SetPointsSet(const PointsSet & p_PointsSet)
{
	m_PointsSet = p_PointsSet;
}

void Unit::Respawn()
{
    TeleportTo(m_RespawnPosition);
}

void Unit::TeleportTo(const WorldPosition& p_WorldPosition)
{
    GetMovementHandler()->ClearMovementStack();
    GetMovementHandler()->StopMovement();
    GetMovementHandler()->StopAttack();

    SetPosX(p_WorldPosition.GetPosX());
    SetPosY(p_WorldPosition.GetPosY());
    SetOrientation(p_WorldPosition.GetOrientation());
}

/// COMBAT
bool Unit::IsInCombat() const
{
    return m_InCombat;
}

void Unit::InCombat()
{
    m_InCombat = true;
    m_CombatTimer = 0;
}

void Unit::OutOfCombat()
{
    //m_MovementHandler->StopAttack();
    m_InCombat = false;
    m_Victim = nullptr;
    CleanAttackers();
    CleanVictims();
}

void Unit::EnterInCombat(Unit* p_Victim)
{
    p_Victim->AddAttacker(this);
    SetVictim(p_Victim);
    AddVictim(p_Victim);
    InCombat();
    p_Victim->InCombat();
}

void Unit::EnterInEvade()
{
    m_Evade = true;
}

void Unit::OutOfEvade()
{
    m_Evade = false;
  //  m_MovementHandler->SetStopPoint(false);
}

bool Unit::IsInEvade() const
{
    return m_Evade;
}

bool Unit::CanAttack(Unit* p_Unit) const
{
    if (!p_Unit->IsInWorld() || p_Unit->IsDeath() || !p_Unit->IsInWorld())
        return false;

    if (GetFaction() == eFactionType::Neutral || p_Unit->GetFaction() == eFactionType::Neutral)
        return false;

    if (this == p_Unit)
        return false;

    if (IsPlayer() && p_Unit->IsPlayer())
        return true;

    if (GetFaction() != p_Unit->GetFaction())
        return true;

    return false;
}

bool Unit::IsHostileTo(Unit* p_Unit)
{
    /// TODO
    return false;
}

void Unit::SetGossipList(std::vector<Gossip>* p_GossipList)
{
   for (Gossip l_Gossip : *p_GossipList)
    {
       /// TODO Free on delete Unit
        m_ListGossip[l_Gossip.m_GossipType].push_back(new Gossip(l_Gossip.m_ID, l_Gossip.m_TypeUnit, l_Gossip.m_UnitEntry, l_Gossip.m_GossipType, l_Gossip.m_Data1, l_Gossip.m_Msg));
    }

}

void Unit::GossipTo(Player* p_Player)
{
    for (Gossip* l_Gossip : m_ListGossip[eGossipType::Whisp])
        p_Player->SendMsg(GetName() + ": " + l_Gossip->m_Msg);
}

void Unit::AddAttacker(Unit* p_Attacker) 
{
    if (m_Attackers.find(p_Attacker) == m_Attackers.end())
        m_Attackers[p_Attacker] = 0;
}

void Unit::AddVictim(Unit* p_Victim)
{
    if (m_Victims.find(p_Victim) == m_Victims.end())
        m_Victims[p_Victim] = 0;
}

void Unit::AddThreadFromAttacker(Unit* p_Attacker, uint16 l_Damage)
{
    if (l_Damage <= 0)
        return;

    if (m_Attackers.find(p_Attacker) != m_Attackers.end())
        m_Attackers[p_Attacker] = m_Attackers[p_Attacker] + l_Damage;
}

void Unit::RemoveAttacker(Unit* p_Attacker)
{
    if (m_Attackers.find(p_Attacker) != m_Attackers.end())
        m_Attackers.erase(p_Attacker);
}

void Unit::RemoveVictim(Unit* p_Victim)
{
    if (m_Victims.find(p_Victim) != m_Victims.end())
        m_Victims.erase(p_Victim);
}

void Unit::SetVictim(Unit* p_Victim) { m_Victim = p_Victim; }

Unit* Unit::GetMaxThreatAttacker()
{
    Unit* l_Attacker = nullptr;
    uint16 l_Thread = 0;
    for (std::map<Unit*, uint16>::iterator l_It = m_Attackers.begin(); l_It != m_Attackers.end(); ++l_It)
    {
        if ((*l_It).second >= l_Thread && CanAttack((*l_It).first))
        {
            l_Attacker = (*l_It).first;
            l_Thread = (*l_It).second;
        }
    }
    return l_Attacker;
}
Unit* Unit::GetVictim() const { return m_Victim; }

uint8 Unit::GetNbAttacker() const
{
    return (uint8)m_Attackers.size();
}

void Unit::UpdateVictims()
{
    for (std::map<Unit*, uint16>::iterator l_It = m_Attackers.begin(); l_It != m_Attackers.end(); ++l_It)
    {
        Unit* l_Attacker = (*l_It).first;
        if (l_Attacker->GetDistance(this) > CaseToPixel(30))
            l_Attacker->RemoveVictim(this);
    }
    for (std::map<Unit*, uint16>::iterator l_It = m_Victims.begin(); l_It != m_Victims.end(); ++l_It)
    {
        Unit* l_Victim = (*l_It).first;
        if (l_Victim->GetDistance(this) > CaseToPixel(30))
            l_Victim->RemoveAttacker(this);
    }
}

void Unit::CleanAttackers()
{
    for (std::map<Unit*, uint16>::iterator l_It = m_Attackers.begin(); l_It != m_Attackers.end(); ++l_It)
    {
        Unit* l_Attacker = (*l_It).first;
        if (l_Attacker && l_Attacker->GetVictim() == this)
            l_Attacker->SetVictim(nullptr);
        if (l_Attacker)
            l_Attacker->RemoveVictim(this);
    }
    m_Attackers.clear();
}

void Unit::CleanVictims()
{
    if (GetVictim())
        GetVictim()->RemoveAttacker(this);
    for (std::map<Unit*, uint16>::iterator l_It = m_Victims.begin(); l_It != m_Victims.end(); ++l_It)
    {
        Unit* l_Victim = (*l_It).first;
        if (l_Victim)
            l_Victim->RemoveAttacker(this);
    }
    m_Victims.clear();
}
#include "Unit.hpp"
#include "../Map/Map.hpp"
#include "../World/WorldSocket.hpp"
#include "../World/PacketDefine.hpp"
#include "../System/Resource/ResourceHealth.hpp"
#include "../System/Resource/ResourceMana.hpp"
#include "../System/Resource/ResourceAlignment.hpp"
#include "../System/Spell/Spell.hpp"
#include "../Global.hpp"
#include <cstdlib>
#include <new>  

Unit::Unit(uint16 p_ID)
{
    Unit(p_ID, TypeUnit::CREATURE, eFactionType::Neutral);
}

Unit::Unit(uint16 p_ID, TypeUnit p_Type, eFactionType p_FactionType)
{
    /// Constructor

    m_Type = p_Type;
    m_FactionType = p_FactionType;
    m_Name = "";
    m_MapID = 0;
    m_ID = p_ID;
    m_SizeX = 24;
    m_SizeY = 32;
    m_Speed = 1.0f;
    m_Resources[eResourceType::Health] = new ResourceHealth();
    m_Resources[eResourceType::Mana] = new ResourceMana();
    m_Resources[eResourceType::Alignment] = new ResourceAlignment();

    m_InWorld = false;
    m_InCombat = false;
    m_Evade = false;

    m_CombatTimer = 0;
    m_ResTimer = 0;
    m_RespawnTime = 0;
    
    m_Victim = nullptr;
    m_CurrentSpell = nullptr;
    if (m_Type == TypeUnit::CREATURE)
        m_MovementHandler = new MovementHandlerCreature(this, m_SizeX, m_SizeY);
    else
        m_MovementHandler = new MovementHandler(m_SizeX, m_SizeY);
}

Player* Unit::ToPlayer()
{
    /// Convert to Player

    if (m_Type == TypeUnit::PLAYER)
        return  reinterpret_cast<Player*>(this);
    else
        return nullptr;
}

Creature* Unit::ToCreature()
{
    /// Convert to Creature

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

    for (auto l_Resource : m_Resources)
        delete l_Resource.second;
}

void Unit::UpdateDeathState(sf::Time p_Diff)
{
    /// Check if Unit is death to respawn it after timer

    if (IsDeath())
    {
        InterruptCast();
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
    /// Update timer of Combat

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
    /// Update differents timers of Gossips

    if (!IsInWorld())
        return;

    if (IsDeath())
        return;

    for (std::vector<Gossip>::iterator l_It = m_ListGossip[eGossipType::Yell].begin(); l_It != m_ListGossip[eGossipType::Yell].end(); ++l_It)
    {
        (*l_It).m_GossipTimer += p_Diff.asMicroseconds();
        if ((*l_It).m_GossipTimer >= (*l_It).m_Data1 * IN_MICROSECOND)
        {
            Talk((*l_It).m_Msg);
            (*l_It).m_GossipTimer -= (*l_It).m_Data1 * IN_MICROSECOND;
        }
    }
}


void Unit::UpdateRegen(sf::Time p_Diff)
{
    /// Update the differents resources
    /// If resource are diferents after Update and Unit is a Player, it's send to player

    if (!IsInWorld())
        return;

    if (IsDeath())
        return;

    for (auto l_Resource : m_Resources)
    {
        uint8 l_Before = l_Resource.second->GetNumber();
        l_Resource.second->Update(p_Diff);
        if (l_Before != l_Resource.second->GetNumber()) ///< Hacky to send for player when resource change
            SetResourceNb(l_Resource.first, l_Resource.second->GetNumber());
    }
}

void Unit::UpdateSpell(sf::Time p_Diff)
{
    /// Update current spell and check if it's ready to launch
    if (m_CurrentSpell == nullptr)
        return;

    uint64 m_ActualTimer = m_CurrentSpell->GetCastTime();
    if (p_Diff.asMicroseconds() >= m_ActualTimer)
        m_CurrentSpell->SetCastTime(0);
    else
        m_CurrentSpell->SetCastTime(m_ActualTimer - p_Diff.asMicroseconds()); ///< /1000 to convert micro to milli

    if (m_CurrentSpell->IsReadyToLaunch())
    {
        /* Send caster visual */
        if (m_CurrentSpell->GetTemplate()->GetVisualID() >= 0)
        {
            PacketUnitPlayVisual l_Packet;
            l_Packet.BuildPacket(GetType(), GetID(), (uint8)m_CurrentSpell->GetTemplate()->GetVisualID());
            m_Map->SendToSet(l_Packet.m_Packet, this);
        }

        m_CurrentSpell->LaunchEffects();
        m_CurrentSpell = nullptr;
    }
}

void Unit::UpdateCooldowns(sf::Time p_Diff)
{
    /// Update spells cooldowns

    for (std::map< uint16, uint64 >::iterator l_It = m_ListSpellID.begin(); l_It != m_ListSpellID.end(); ++l_It)
    {
        if ((*l_It).second == 0)
            continue;

        if (p_Diff.asMicroseconds() >= (int64)(*l_It).second)
        {
            (*l_It).second = 0;
            continue;
        }
        
        (*l_It).second -= p_Diff.asMicroseconds();
    }
}

void Unit::Update(sf::Time p_Diff)
{
    ///  Launch every updates
    UpdateDeathState(p_Diff);
    UpdateCombat(p_Diff);
    UpdateRegen(p_Diff);
    UpdateGossip(p_Diff);
    UpdateCooldowns(p_Diff);
    UpdateSpell(p_Diff);
    UpdateVictims();

    if (m_MovementHandler == nullptr)
        return;

    if (!IsInWorld())
        return;

    m_MovementHandler->Update(p_Diff);

    if (GetPosX() != m_MovementHandler->GetPosX() || GetPosY() != m_MovementHandler->GetPosY() || m_MovementHandler->IsInAttack())
    {
        if (((m_MovementHandler->GetPosY() / TILE_SIZE) * m_Map->GetSizeX()) + (m_MovementHandler->GetPosX() / TILE_SIZE) != ((GetPosY() / TILE_SIZE) * m_Map->GetSizeX()) + (GetPosX() / TILE_SIZE))
            m_Map->GetCase(((m_MovementHandler->GetPosY() / TILE_SIZE) * m_Map->GetSizeX()) + (m_MovementHandler->GetPosX() / TILE_SIZE))->UnitEnterInCase(this);
        SetPosX(m_MovementHandler->GetPosX());
        SetPosY(m_MovementHandler->GetPosY());
        InterruptCast(); /// Interrupt Cast on Movement
    }

    if (m_MovementHandler->IsDamageReady())
    {
        Unit* l_Unit = m_Map->GetCloserUnit(this, MELEE_RANGE, true, true ,true);

        if (l_Unit != nullptr)
            AutoAttack(l_Unit);

        m_MovementHandler->SetDamageDone(true);
    }
}

bool Unit::IsInFront(const Position & p_Position) const
{
    /// Check if Unit is in front of a position
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

bool Unit::IsInFront(Unit const* p_Unit) const
{
    /// Check if Unit is in front of a Unit
    return IsInFront(p_Unit->GetPosition());
}

void Unit::DealHeal(Unit* p_Victim, DamageInfo p_DamageInfo)
{
    /// Heal a target for the value in DamageInfo
    /// Log are send to victim if it's a player
    if (p_DamageInfo.m_Result == DamageResult::Miss)
        p_DamageInfo.m_Damage = 0;

    int8 l_NewHealth = std::min(p_Victim->GetResourceNb(eResourceType::Health) - p_DamageInfo.m_Damage, 100);

    if (IsPlayer())
        ToPlayer()->GetSession()->SendLogDamage(p_Victim->GetType(), p_Victim->GetID(), p_DamageInfo);

    p_Victim->SetResourceNb(eResourceType::Health, (uint8)l_NewHealth);
    switch (p_Victim->GetType())
    {
    case TypeUnit::PLAYER:
        if (p_Victim != this)
            p_Victim->ToPlayer()->GetSession()->SendLogDamage(p_Victim->GetType(), p_Victim->GetID(), p_DamageInfo);
        break;
        /*case TypeUnit::CREATURE:
        p_Victim->ToCreature()->SetHealth((uint8)l_NewHealth);
        break;*/
    default:
        break;
    }
}

void Unit::DealDamage(Unit* p_Victim, DamageInfo p_DamageInfo)
{
    /// Damage a target for the value in DamageInfo
    /// Log are send to victim if it's a player
    if (p_DamageInfo.m_Result == DamageResult::Miss)
        p_DamageInfo.m_Damage = 0;

    int8 l_NewHealth = std::max(p_Victim->GetResourceNb(eResourceType::Health) - p_DamageInfo.m_Damage, 0);

    if (IsPlayer())
        ToPlayer()->GetSession()->SendLogDamage(p_Victim->GetType(), p_Victim->GetID(), p_DamageInfo);

    p_Victim->SetResourceNb(eResourceType::Health, (uint8)l_NewHealth);
    switch (p_Victim->GetType())
    {
    case TypeUnit::PLAYER:
        if (p_Victim != this)
            p_Victim->ToPlayer()->GetSession()->SendLogDamage(p_Victim->GetType(), p_Victim->GetID(), p_DamageInfo);
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
            Player* l_Player = ToPlayer();
            Creature* l_Victim = p_Victim->ToCreature();
            l_Player->SetXp(l_Player->GetXp() + l_Victim->GetXpEarn());
            l_Player->CheckQuestObjective(eObjectifType::KillMob, l_Victim->GetEntry());
        }
        if (IsPlayer() && p_Victim->IsPlayer())
        {
            Player* l_Player = ToPlayer();
            l_Player->AddResourceNb(eResourceType::Alignment, -15);
        }
        return;
    }

    if (!p_DamageInfo.m_Damage)
        return;

    if (GetVictim() != p_Victim)
    {
        EnterInCombat(p_Victim);
        p_Victim->AddThreadFromAttacker(this, p_DamageInfo.m_Damage);
    }
    else
    {
        InCombat();
        p_Victim->InCombat();
        p_Victim->AddThreadFromAttacker(this, p_DamageInfo.m_Damage);
    }
}

void Unit::AutoAttack(Unit* p_Victim)
{
    /// This fonction is call everytime autoattack are ready 
    /// It's calling deal damage

	uint16 l_ForceAttacker = m_PointsSet.m_Force;
	uint16 l_StaminaDefenser = p_Victim->GetPointsSet().m_Stamina;
	int16 l_Balance = l_ForceAttacker - l_StaminaDefenser;
    int8 l_Damage = rand() %  2 + 10;
	l_Damage += (l_Balance * 2);
	l_Damage = std::max(l_Damage, (int8)0);

	uint16 l_DexterityAttacker = m_PointsSet.m_Dexterity;
	uint16 l_DexterityDefenser = p_Victim->GetPointsSet().m_Dexterity;
	l_Balance = l_DexterityDefenser - l_DexterityAttacker;
	int8 l_MissChance = 20 + (l_Balance * 4);
	l_MissChance = std::max(l_MissChance, (int8)0);
	l_MissChance = std::min(l_MissChance, (int8)100);
	bool l_Miss = (rand() % 100) <= l_MissChance;
	
    DamageInfo l_DamageInfo;
    if (l_Miss)
        l_DamageInfo.m_Result = DamageResult::Miss;
    l_DamageInfo.m_Damage = l_Damage;
    DealDamage(p_Victim, l_DamageInfo);
}

bool Unit::IsInWorld() const
{
    /// Check if Unit is actually in world

    return m_InWorld;
}

void Unit::SetInWorld(bool p_InWorld)
{
    /// Set the inworld state of Unit

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
    if (m_Resources.find(p_Resource) == m_Resources.end())
        return nullptr;
    return m_Resources[p_Resource];
}

uint8 Unit::GetResourceNb(eResourceType p_Resource)
{
    if (m_Resources.find(p_Resource) == m_Resources.end())
        return 0;
    return m_Resources[p_Resource]->GetNumber();
}

void Unit::SetResourceNb(eResourceType p_Resource, uint8 p_Nb)
{
    if (m_Resources.find(p_Resource) == m_Resources.end())
        return;

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

void Unit::AddResourceNb(eResourceType p_Resource, uint8 p_Nb)
{
    if (m_Resources.find(p_Resource) == m_Resources.end())
        return;

    m_Resources[p_Resource]->AddNumber(p_Nb);

    switch (p_Resource)
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
    InterruptCast();
    SetPosX(p_WorldPosition.GetPosX());
    SetPosY(p_WorldPosition.GetPosY());
    SetOrientation(p_WorldPosition.GetOrientation());
    SetMapID(p_WorldPosition.GetMapID());

    /* TODO : check range for short TP */
    CleanAttackers();
    CleanVictims();

    PacketUpdatePosition l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(), GetMapID(), GetPosX(), GetPosY(), GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, this);

    /* To be sure that some packet are not receive between tp */
    if (IsPlayer())
        ToPlayer()->SetInLoading(true);
}

void Unit::TeleportTo(uint32 p_X, uint32 p_Y, Orientation p_Orientation)
{
    TeleportTo(WorldPosition(p_X, p_Y, GetMapID(), p_Orientation));
}

void Unit::TeleportTo(uint16 p_MapID, uint32 p_X, uint32 p_Y, Orientation p_Orientation)
{
    if (IsPlayer())
        TeleportTo(WorldPosition(p_X, p_Y, p_MapID, p_Orientation));
}

bool Unit::IsInCombat() const
{
    /// Check if Unit is in Combat state
    return m_InCombat;
}

void Unit::InCombat()
{
    /// Reset combat timer to 0
    /// Call at each combat action

    m_InCombat = true;
    m_CombatTimer = 0;
}

void Unit::OutOfCombat()
{
    /// Out of combat
    /// Remove all the attackers, victims ...

    m_InCombat = false;
    m_Victim = nullptr;
    CleanAttackers();
    CleanVictims();
}

void Unit::EnterInCombat(Unit* p_Victim)
{
    /// Enter in combat state

    p_Victim->AddAttacker(this);
    SetVictim(p_Victim);
    AddVictim(p_Victim);
    InCombat();
    p_Victim->InCombat();
}

void Unit::EnterInEvade()
{
    /// Enter in Evade state, it's no more possible to target this Unit

    RegenerateAll();
    m_Evade = true;
}

void Unit::RegenerateAll()
{
    /// Regenerate all resources

    for (auto l_Resource : m_Resources)
    {
        if (l_Resource.second->GetNumber() == 100) /// Already max
            continue;

        SetResourceNb(l_Resource.first, 100);
    }
}

void Unit::OutOfEvade()
{
    /// Out of evade state

    m_Evade = false;
  /*  m_MovementHandler->SetStopPoint(false);*/
}

bool Unit::IsInEvade() const
{
    /// Check if Unit is in Evade state

    return m_Evade;
}

bool Unit::CanAttack(Unit* p_Unit) const
{
    /// Check if target is a valid attack target

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
    /// Set the gossip list of Unit

   for (Gossip l_Gossip : *p_GossipList)
    {
       m_ListGossip[l_Gossip.m_GossipType].push_back(l_Gossip);
    }

}

void Unit::GossipTo(Player* p_Player)
{
    /// Send a gossip wisp to a Player
    /// We launch actions of wisp here

    if (IsPlayer())
        return;

    std::string l_GossipMsg = "";
    uint32 l_Data1 = 0;

    if (IsCreature())
        p_Player->CheckQuestObjective(eObjectifType::SpeackMob, ToCreature()->GetEntry());

    /* QUEST VALIDATE*/
    for (Gossip l_Gossip : m_ListGossip[eGossipType::ValidQuest]) ///< Only one Wisp can be done
    {
        if (l_Gossip.m_Required != nullptr && (!l_Gossip.m_Required->IsValid(p_Player)))
            continue;
        l_GossipMsg = l_Gossip.m_Msg;
        l_Data1 = l_Gossip.m_Data1;
        if (l_Gossip.m_Required != nullptr && l_Gossip.m_Required->IsValid(p_Player)) ///< Priority for Wisp with Valid Required
            break;
    }
    if (l_GossipMsg != "")
    {
        Quest* l_Quest = p_Player->GetQuest(l_Data1);
        if (l_Quest == nullptr)
            return;

        p_Player->ValidateQuest(l_Quest);
        p_Player->SendMsg(GetName() + ": " + l_GossipMsg);
        return;
    }
    /* QUEST LAUNCHER*/
    for (Gossip l_Gossip : m_ListGossip[eGossipType::LaunchQuest]) ///< Only one Wisp can be done
    {
        if (l_Gossip.m_Required != nullptr && (!l_Gossip.m_Required->IsValid(p_Player) || p_Player->HasQuestInProgress(l_Gossip.m_Data1)))
            continue;

        l_Data1 = l_Gossip.m_Data1;
        if (g_SqlManager->GetDaysSinceLastQuestDone(p_Player, l_Data1) >= 0) ///< TODO check repetition
            continue; 
        if (p_Player->GetQuest(l_Data1) != nullptr) ///< Only if we have not it already
            continue;
        l_GossipMsg = l_Gossip.m_Msg;
        if (l_Gossip.m_Required != nullptr && l_Gossip.m_Required->IsValid(p_Player)) ///< Priority for Wisp with Valid Required
            break;
    }
    if (l_GossipMsg != "")
    {
        Quest* l_Quest = nullptr;
        try
        {
            l_Quest = new Quest(g_QuestManager->GetQuestTemplate(l_Data1));
        }
        catch (std::bad_alloc& ba)
        {
            printf("-> bad_alloc on quest caught: %d\n", ba.what());
        }
        p_Player->AddQuest(l_Quest);
        p_Player->SendMsg(GetName() + ": " + l_GossipMsg);
        return;
    }

    /* SIMPLE WHISP */
    for (Gossip l_Gossip : m_ListGossip[eGossipType::Whisp]) ///< Only one Wisp can be done
    {
        if (l_Gossip.m_Required != nullptr && !l_Gossip.m_Required->IsValid(p_Player))
            continue;
        l_GossipMsg = l_Gossip.m_Msg;
        if (l_Gossip.m_Required != nullptr && l_Gossip.m_Required->IsValid(p_Player)) ///< Priority for Wisp with Valid Required
            break;
    }
    if (l_GossipMsg != "")
        p_Player->SendMsg(GetName() + ": " + l_GossipMsg);

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

void Unit::AddSpellID(uint16 p_SpellID, uint64 p_Cooldown)
{
    m_ListSpellID[p_SpellID] = 0;
    AddSpellCooldown(p_SpellID, p_Cooldown);
}

std::map< uint16, uint64 >* Unit::GetSpellList()
{
    return &m_ListSpellID;
}

void Unit::AddSpellCooldown(uint16 p_SpellID, uint64 p_Cooldown)
{
    if (m_ListSpellID.find(p_SpellID) == m_ListSpellID.end())
        return;

    m_ListSpellID[p_SpellID] += p_Cooldown;
}

bool Unit::HasSpellCooldown(uint16 p_SpellID)
{
    if (m_ListSpellID.find(p_SpellID) == m_ListSpellID.end())
        return false;

    if (m_ListSpellID[p_SpellID] > 0)
        return true;

    return false;
}

void Unit::CastSpell(uint16 p_ID)
{
    if (g_SpellManager == nullptr)
        return;

    SpellTemplate* l_SpellTemplate = g_SpellManager->GetSpell(p_ID);
    if (l_SpellTemplate == nullptr)
        return;

    Spell* l_Spell = nullptr;
    try
    {
        l_Spell = new Spell(l_SpellTemplate);
    }
    catch (std::bad_alloc& ba)
    {
        printf("-> bad_alloc on spell caught: %d\n", ba.what());
    }
    if (!l_Spell->Prepare(this))
        return;

    InterruptCast();
    SetCurrentSpell(l_Spell);
}

void Unit::SetCurrentSpell(Spell* p_Spell)
{
    m_CurrentSpell = p_Spell; /// Casting

    if (p_Spell->GetCastTime() <= 0)
        return;

    /// Send Cast Bar visual
    PacketUnitCastBar l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(), (uint8)((p_Spell->GetCastTime() / 1000) / 100));
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Unit::InterruptCast()
{
    if (m_CurrentSpell == nullptr)
        return;
    
    m_CurrentSpell->Interrupt();
    m_CurrentSpell = nullptr;
    /// Send Cast Bar visual
    PacketUnitCastBar l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(), 0);
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

Spell* Unit::GetCurrentSpell() const
{
    return m_CurrentSpell;
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

float Unit::GetSpeed() const
{
    return m_MovementHandler->GetSpeed();
}

uint8 Unit::GetSpeedUint8() const
{
    return (uint8)(m_MovementHandler->GetSpeed() * 10.0f);
}

void Unit::SetSpeed(float p_Speed)
{
    m_MovementHandler->SetSpeed(p_Speed);

    PacketUnitUpdateSpeed l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(), (uint8)(p_Speed * 10.0f));
    m_Map->SendToSet(l_Packet.m_Packet, this);
}
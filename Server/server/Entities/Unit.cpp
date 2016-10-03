#include "Unit.hpp"
#include "../Map/Map.hpp"
#include <cstdlib>


Unit::Unit(uint16 p_ID)
{
    Unit::Unit(p_ID, TypeUnit::CREATURE);
}

Unit::Unit(uint16 p_ID, TypeUnit p_Type)
{
    m_Type = p_Type;
    m_Name = "";
    m_MapID = 0;
    m_ID = p_ID;
    m_SizeX = 24;
    m_SizeY = 32;
    m_Health = 100;

    m_InWorld = true;
    m_InCombat = false;
    m_Evade = false;

    m_CombatTimer = 0;
    m_ResTimer = 0;
    m_RespawnTime = 0;
    
    m_Victim = nullptr;
    m_Attacker = nullptr;
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
    m_MovementHandler->StopMovement();
    m_MovementHandler->StopAttack();
    m_Map->RemoveUnit(this);
    delete m_MovementHandler;
}

void Unit::UpdateDeathState(sf::Time p_Diff)
{
    if (IsDeath())
    {
        m_ResTimer += p_Diff.asMicroseconds();
        if (m_ResTimer >= m_RespawnTime)
        {
            switch (m_Type)
            {
            case TypeUnit::PLAYER:
                ToPlayer()->Respawn();
                break;
            case TypeUnit::CREATURE:
                ToCreature()->Respawn();
                break;
            default:
                break;
            }
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

void Unit::Update(sf::Time p_Diff)
{
    UpdateDeathState(p_Diff);
    UpdateCombat(p_Diff);

    if (m_MovementHandler == nullptr)
        return;

    if (!IsInWorld())
        return;

    m_MovementHandler->Update(p_Diff);
    SetPosX(m_MovementHandler->GetPosX());
    SetPosY(m_MovementHandler->GetPosY());

    if (m_MovementHandler->IsDamageReady())
    {
        Unit* l_Unit = m_Map->GetCloserUnit(this, MELEE_RANGE, true);

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
    uint8 l_Damage = rand() %  8 + 8;
    int8 l_NewHealth = std::max(p_Victim->GetHealth() - l_Damage, 0);

    if (!l_Damage)
        return;

    switch (p_Victim->GetType())
    {
        case TypeUnit::PLAYER:
            p_Victim->ToPlayer()->SetHealth((uint8)l_NewHealth);
        break;
        case TypeUnit::CREATURE:
            p_Victim->ToCreature()->SetHealth((uint8)l_NewHealth);
            break;
        default:
            break;
    }

    if (p_Victim->IsDeath())
        return;

    if (GetVictim() != p_Victim)
        EnterInCombat(p_Victim);
    else
    {
        InCombat();
        p_Victim->InCombat();
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

uint8 Unit::GetHealth() const
{
    return m_Health;
}

uint8 Unit::GetSkinID() const
{
    return m_SkinID;
}

bool Unit::IsPlayer() const
{
    return m_Type == TypeUnit::PLAYER;
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

void Unit::SetHealth(const uint8 & p_Health)
{
    m_Health = p_Health;

    if (!m_Health)
        OutOfCombat();
}

bool Unit::IsDeath() const
{
    if (m_Health)
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

void Unit::Respawn()
{
    SetPosX(m_RespawnPosition.GetPosX());
    SetPosY(m_RespawnPosition.GetPosY());
    SetOrientation(m_RespawnPosition.GetOrientation());
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
    m_InCombat = false;
    m_Attacker = nullptr;
    m_Victim = nullptr;
}

void Unit::EnterInCombat(Unit* p_Victim)
{
    p_Victim->SetAttacker(this);
    SetVictim(p_Victim);
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
}

bool Unit::IsInEvade() const
{
    return m_Evade;
}

void Unit::SetAttacker(Unit* p_Attacker) { m_Attacker = p_Attacker; }
void Unit::SetVictim(Unit* p_Victim) { m_Victim = p_Victim; }
Unit* Unit::GetAttacker() const { return m_Attacker; }
Unit* Unit::GetVictim() const { return m_Victim; }
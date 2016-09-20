#include "Unit.hpp"
#include "../Map/Map.hpp"
#include <cstdlib>


Unit::Unit(uint16 p_ID)
{
    m_Name = "";
    m_MapID = 0;
    m_Type = TypeUnit::CREATURE;
    m_ID = p_ID;
    m_SizeX = 24;
    m_SizeY = 32;
    m_MovementHandler = new MovementHandler(m_SizeX, m_SizeY);
    m_Health = 100;
}

Unit::Unit(uint16 p_ID, TypeUnit p_Type)
{
    m_Type = p_Type;
    m_Name = "";
    m_MapID = 0;
    m_ID = p_ID;
    m_SizeX = 24;
    m_SizeY = 32;
    m_MovementHandler = new MovementHandler(m_SizeX, m_SizeY);
    m_Health = 100;
}

Player* Unit::ToPlayer()
{
    if (m_Type == TypeUnit::PLAYER)
        return  reinterpret_cast<Player*>(this);
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

void Unit::Update(sf::Time p_Diff)
{
    if (m_MovementHandler == nullptr)
        return;

    m_MovementHandler->Update(p_Diff);
    SetPosX(m_MovementHandler->GetPosX());
    SetPosY(m_MovementHandler->GetPosY());

    if (m_MovementHandler->IsDamageReady())
    {
        Unit* l_Unit = m_Map->GetCloserUnit(this, this->GetSizeX(), true);

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
            if (p_Position.y >= GetPosY())
                return true;
            break;
        case Orientation::Left:
            if (p_Position.x <= GetPosX())
                return true;
            break;
        case Orientation::Right:
            if (p_Position.x >= GetPosX())
                return true;
            break;
        case Orientation::Up:
            if (p_Position.y <= GetPosY())
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
    int16 l_NewHealth = p_Victim->GetHealth() - l_Damage;
    if (l_NewHealth < 0)
        l_NewHealth = 0;
    p_Victim->ToPlayer()->SetHealth((uint8)l_NewHealth);
}

bool Unit::IsInFront(Unit const* p_Unit) const
{
    return IsInFront(p_Unit->GetPosition());
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

void Unit::SetPoxY(const uint32 & p_PosY)
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
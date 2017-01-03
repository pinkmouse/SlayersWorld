#include "Unit.hpp"
#include "../Map/Map.hpp"

Unit::Unit(uint16 p_ID) :
    WorldObject(TypeWorldObject::UNIT, 24, 32)
{
    Unit(p_ID, TypeUnit::CREATURE);
}

Unit::Unit(uint16 p_ID, TypeUnit p_Type) :
    WorldObject(TypeWorldObject::UNIT, 24, 32)
{
    m_Name = "";
    m_Type = p_Type;
    m_ID = p_ID;
    m_Opacity = 0;
    m_DiffTimeOpactiy = 0;
    m_MovementHandler = new MovementHandler(GetSizeX(), GetSizeY());
    m_SkinZoomFactor = SKIN_ZOOM_FACTOR_DEFAULT;
}

Unit::~Unit()
{
    delete m_MovementHandler;
}

bool Unit::IsDeath() const
{
    if (m_Health > 0)
        return false;
    return true;
}

bool Unit::IsPlayer() const
{
    return m_Type == TypeUnit::PLAYER;
}

MovementHandler* Unit::GetMovementHandler()
{
    return m_MovementHandler;
}

void Unit::Update(sf::Time p_Diff)
{
    m_MovementHandler->Update(p_Diff);
    SetPosX(m_MovementHandler->GetPosX());
    SetPosY(m_MovementHandler->GetPosY());

    if (m_Opacity < MAX_OPACITY)
    {
        m_DiffTimeOpactiy += p_Diff.asMicroseconds();
        while (m_DiffTimeOpactiy > (uint64)(UPDATE_OPACITY_TIME * 1000)) ///< 1000 because microsecond
        {
            /// UPDATE OPACITY
            if ((int16)m_Opacity + 15 > MAX_OPACITY)
                m_Opacity = MAX_OPACITY;
            else
                m_Opacity += 15;
            m_DiffTimeOpactiy -= (uint64)(UPDATE_TIME_MOVEMENT * 1000);
        }
    }

    if (!m_Talk.empty())
    {
        m_DiffTimeTalk += p_Diff.asMicroseconds();
        if (m_DiffTimeTalk > (uint64)(TALK_TIME_SHOW * 1000)) ///< 1000 because microsecond
        {
            m_Talk.clear();
            m_DiffTimeTalk = 0;
        }
    }

	for (std::vector<std::pair<DamageInfo, uint32>>::iterator l_It = m_HistoryDamage.begin(); l_It != m_HistoryDamage.end();)
	{
		if ((*l_It).second <= (p_Diff.asMicroseconds() * 2))
			l_It = m_HistoryDamage.erase(l_It);
		else
		{
			(*l_It).second -= (p_Diff.asMicroseconds() * 2);
			++l_It;
		}
	}
}

uint8 Unit::GetOpacity() const
{
    if (IsDeath())
    {
        switch (GetType())
        {
            case TypeUnit::PLAYER:
                return DEATH_OPACITY;
            default:
                return 0;
        }
    }

    return m_Opacity;
}

std::string Unit::GetName() const
{
    return m_Name;
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

Orientation Unit::GetOrientation() const
{
    return m_MovementHandler->GetOrientation();
}

TypeUnit Unit::GetType() const
{
    return m_Type;
}

Player* Unit::ToPlayer()
{
    if (m_Type == TypeUnit::PLAYER)
        return  reinterpret_cast<Player*>(this);
    else
        return nullptr;
}

uint16 Unit::GetID() const
{
    return m_ID;
}

void Unit::SetMap(Map* p_Map)
{
    m_Map = p_Map;
    m_MovementHandler->SetMap(m_Map);
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

void Unit::SetHealth(const uint8 & p_Health)
{
    m_Health = p_Health;
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
    m_MovementHandler->SetOrientation((Orientation)p_Orientation);
}

bool Unit::IsInRayVisible(Unit* p_Unit)
{
    if (p_Unit->GetMapID() != GetMapID())
        return false;

    if (p_Unit == this)
        return true;

    std::vector<uint16> l_SquareSetID = m_Map->GetSquareSetID(m_Map->GetSquareID(GetCasePosX(), GetCasePosY()));
    uint16 l_UnitSquareID = m_Map->GetSquareID(p_Unit->GetCasePosX(), p_Unit->GetCasePosY());

    for (uint16 l_Id : l_SquareSetID)
    {
        if (l_Id == l_UnitSquareID)
            return true;
    }
    return false;
}

uint16 Unit::GetCasePosX() const
{
    return GetPosX() / TILE_SIZE;
}

uint16 Unit::GetCasePosY() const
{
    return GetPosY() / TILE_SIZE;
}

void Unit::StartMovement()
{
    if (m_MovementHandler == nullptr)
        return;

    m_MovementHandler->StartMovement();
}

void Unit::SetTalk(const std::string & p_Talk)
{
    m_Talk = p_Talk;
    m_DiffTimeTalk = 0;
}

std::string Unit::GetTalk() const
{
    return m_Talk;
}

float Unit::GetSkinZoomFactor() const
{
    return m_SkinZoomFactor;
}

void Unit::SetSkinZoomFactor(const float & p_SkinZoomFactor)
{
    m_SkinZoomFactor = p_SkinZoomFactor;
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

void Unit::AddDamageLog(const DamageInfo & p_Damage)
{
	m_HistoryDamage.push_back(std::pair<DamageInfo, uint32>(p_Damage, MAX_HISTORY_LOG_TIME));
}

std::vector<std::pair<DamageInfo, uint32>>& Unit::GetDamageLog()
{
	return m_HistoryDamage;
}
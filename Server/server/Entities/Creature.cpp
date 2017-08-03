#include "Creature.hpp"
#include "../Map/Map.hpp"
#include "../World/PacketDefine.hpp"

Creature::Creature(uint16 p_ID, CreatureTemplate* p_CreatureTemplate, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY) :
    Unit(p_ID, TypeUnit::CREATURE, p_CreatureTemplate->m_FactionType)
{
    m_Entry = p_CreatureTemplate->m_Entry;
    SetMapID(p_MapID);
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    m_SkinID = p_CreatureTemplate->m_SkinID;
    m_Level = p_CreatureTemplate->m_Level;
    m_Name = p_CreatureTemplate->m_Name;
    m_CreatureTemplate = p_CreatureTemplate;
    m_Static = false;

    m_RespawnPosition.SetMapID(p_MapID);
    m_RespawnPosition.SetPosX(p_PosX);
    m_RespawnPosition.SetPosY(p_PosY);
    m_MovementHandler->SetSpeed((float)p_CreatureTemplate->m_Speed / 10.0f);
    ResetRandMovementTime(false);

    PointsSet l_PointsSet;
    l_PointsSet.SetStat(eStats::Force, m_CreatureTemplate->m_Force);
    l_PointsSet.SetStat(eStats::Stamina, m_CreatureTemplate->m_Stamina);
    l_PointsSet.SetStat(eStats::Dexterity, m_CreatureTemplate->m_Dexterity);
	SetPointsSet(l_PointsSet);
    m_RespawnTime = m_CreatureTemplate->m_RespawnTime * IN_MILLISECOND;
    m_DiffAttackEffect = 0;
    if (p_CreatureTemplate->m_MovingTimeMin == -1)
        m_Static = true;
}

Creature::~Creature()
{
}

uint16 Creature::GetEntry() const
{
    return m_Entry;
}

void Creature::RandMoving()
{
    if (m_Static)
        return;

    ResetRandMovementTime(true);

    if (m_RandMovementTime <= 0)
        return;

    uint8 l_Orientation = rand() % 4;
    if (GetMapID() == m_RespawnPosition.GetMapID() && GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate->m_MaxRay))
        l_Orientation = GetOrientationToPoint(m_RespawnPosition.GetPosition());
    StartMovement((Orientation)l_Orientation);
}

void Creature::StartMovement(Orientation p_Orientation)
{
    if (m_MovementHandler == nullptr)
        return;

    m_MovementHandler->StartMovement(p_Orientation);
}

void Creature::StopMovement()
{
    if (!m_MovementHandler->IsInMovement())
        return;

    m_MovementHandler->StopMovement();
}

void Creature::StartAttack(Unit* p_Victim)
{
    m_MovementHandler->StartAttack();
}

void Creature::StopAttack()
{
    m_MovementHandler->StopAttack();
}

void Creature::UpdateOrientation(Orientation p_Orientation)
{
    if (p_Orientation == m_MovementHandler->GetOrientation())
        return;

    m_MovementHandler->SetOrientation(p_Orientation);
    PacketUpdateOrientation l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID(), GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Creature::Update(sf::Time p_Diff)
{
    Unit::Update(p_Diff);
    UpdateAI(p_Diff);
}

void Creature::SetResourceNb(eResourceType p_Resource, uint8 p_Nb)
{
    Unit::SetResourceNb(p_Resource, p_Nb);
    if (IsDeath())
        Unspawn();
    else
    {
      /*  WorldPacket l_Packet;
        uint8 l_ID = SMSG::S_UnitUpdateResource;

        l_Packet << l_ID << GetType() << GetID() << p_Resource << GetResourceNb(p_Resource);
        m_Map->SendToSet(l_Packet, this);*/
    }
}

void Creature::AddResourceNb(eResourceType p_Resource, uint8 p_Nb)
{
    Unit::AddResourceNb(p_Resource, p_Nb);
    if (IsDeath())
        Unspawn();
    else
    {
        WorldPacket l_Packet;
        uint8 l_ID = SMSG::S_UnitUpdateResource;

        l_Packet << l_ID << GetType() << GetID() << p_Resource << GetResourceNb(p_Resource);
        m_Map->SendToSet(l_Packet, this);
    }
}

uint32 Creature::GetXpEarn() const
{
    return m_CreatureTemplate->m_Xp;
}

void Creature::ReturnToRespawnPoint()
{
    if (GetPosition() == m_RespawnPosition.GetPosition() && IsInMovement())
    {
        StopMovement();
        return;
    }

    if (!IsFollowingPath() && PositionToCasePosition(GetPosition()) != PositionToCasePosition(m_RespawnPosition.GetPosition()))
    {
        m_PathToTargetPosition = m_Map->LaunchPathFinding(PositionToCasePosition(GetPositionCentered()), PositionToCasePosition(m_RespawnPosition.GetPosition()));
    }
    else if (!IsFollowingPath() && PositionToCasePosition(GetPosition()) == PositionToCasePosition(m_RespawnPosition.GetPosition())) ///< WHEN IN GOOD CASE
    {
        m_MovementHandler->SetStopPoint(true, m_RespawnPosition.GetPosition());
        Orientation l_Orientation = GetOrientationToPoint(m_RespawnPosition.GetPosition());
        printf("Return to respawn [%d]\n", l_Orientation);
        if (GetOrientation() != l_Orientation || !IsInMovement())
            StartMovement(l_Orientation);
    }
}

void Creature::ReturnInRay()
{
    if (GetMapID() != m_RespawnPosition.GetMapID())
        return;

    if (IsFollowingPath() && GetDistance(m_RespawnPosition.GetPosition()) <= CaseToPixel(m_CreatureTemplate->m_MaxRay))
    {
        m_PathToTargetPosition.clear();
    }
    else if (!IsFollowingPath() && GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate->m_MaxRay)) ///< WHEN IN GOOD CASE
    {
        if (PositionToCasePosition(GetPosition()) == PositionToCasePosition(m_RespawnPosition.GetPosition()))
        {
            m_MovementHandler->SetStopPoint(true, m_RespawnPosition.GetPosition());

            Orientation l_Orientation = GetOrientationToPoint(m_RespawnPosition.GetPosition());
            if (GetOrientation() != l_Orientation || !IsInMovement())
                StartMovement(l_Orientation);
        }
        else
            m_PathToTargetPosition = m_Map->LaunchPathFinding(PositionToCasePosition(GetPositionCentered()), PositionToCasePosition(m_RespawnPosition.GetPosition()));
    }
}

void Creature::GoToCase(const Position & p_Position)
{
    if (m_PathToTargetPosition.empty() || m_PathToTargetPosition[0] != PositionToCasePosition(p_Position))
    {
        m_PathToTargetPosition = m_Map->LaunchPathFinding(PositionToCasePosition(GetPositionCentered()), PositionToCasePosition(p_Position));
    }
}

void Creature::ResetRandMovementTime(bool ForMoving)
{
    m_DiffMovementTime = 0;
    if (ForMoving)
    {
        if (m_CreatureTemplate->m_MovingTimeMax - m_CreatureTemplate->m_MovingTimeMin <= 0)
            m_RandMovementTime = m_CreatureTemplate->m_MovingTimeMax;
        else
            m_RandMovementTime = m_CreatureTemplate->m_MovingTimeMin + ((float)(rand() % (uint16)((float)(m_CreatureTemplate->m_MovingTimeMax - m_CreatureTemplate->m_MovingTimeMin) * 100.0f)) / 100.0f);
    }
    else
    {
        if (m_CreatureTemplate->m_StopTimeMax - m_CreatureTemplate->m_StopTimeMin <= 0)
            m_RandMovementTime = m_CreatureTemplate->m_StopTimeMax;
        else
            m_RandMovementTime = m_CreatureTemplate->m_StopTimeMin + ((float)(rand() % (uint16)((float)(m_CreatureTemplate->m_StopTimeMax - m_CreatureTemplate->m_StopTimeMin) * 100.0f)) / 100.0f);
    }
    if (m_RandMovementTime < 0)
        m_RandMovementTime = 0;
}

void Creature::Unspawn()
{
    Unit::Unspawn();
    ResetRandMovementTime(false);
}

void Creature::Respawn()
{
    Unit::Respawn();
    m_DiffMovementTime = 0;
    m_DiffAttackEffect = 0;
    SetInWorld(true);

    SetResourceNb(eResourceType::Health, MAX_HEALTH);
    m_ResTimer = 0;

    /// Respawn creature for players
    PacketUnitCreate l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID(), GetName(), GetLevel(), GetResourceNb(eResourceType::Health), GetResourceNb(eResourceType::Mana), GetResourceNb(eResourceType::Alignment), GetSkinID(), GetSizeX(), GetSizeY(), GetSpeedUint8(), GetMapID(), GetPosition(), GetOrientation(), m_MovementHandler->IsInMovement(), false);
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

bool Creature::IsFollowingPath() const
{
    return !m_PathToTargetPosition.empty();
}

Orientation Creature::GetOrientationByPath(Path & p_Path)
{
    if (p_Path.empty())
    {
        StopMovement();
        return m_MovementHandler->GetOrientation();
    }
    Position p_NextPos = p_Path[p_Path.size() - 1];

    uint8 m_Offset = 0;
    switch (GetOrientation())
    {
        case Orientation::Left :
            m_Offset = -10;
            break;
        case Orientation::Right:
            m_Offset = 10;
            break;
        default:
            break;
    }

    if (GetPositionCentered().m_X / TILE_SIZE == p_NextPos.m_X &&
        GetPositionCentered().m_Y / TILE_SIZE == p_NextPos.m_Y)
    {
        if (!IsCenteredInCase())
            return OrientationToBeCenteredInCase(p_NextPos);

        p_Path.pop_back();

        if (p_Path.empty())
        {
            StopMovement();
            return m_MovementHandler->GetOrientation();
        }
        p_NextPos = p_Path[p_Path.size() - 1];
        printf("#### Go TO : [%d-%d]\n", p_NextPos.m_X, p_NextPos.m_Y);
    }
    //printf("#### [%d-%d]\n", p_NextPos.m_X, p_NextPos.m_Y);
    Orientation l_Orien = GetOrientationToCase(GetPositionCentered(), p_NextPos);

    return l_Orien;
}
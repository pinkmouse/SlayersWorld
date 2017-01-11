#include "Creature.hpp"
#include "../Map/Map.hpp"
#include "../World/PacketDefine.hpp"

Creature::Creature(uint16 p_ID, uint16 p_Entry, CreatureTemplate p_CreatureTemplate, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY) :
    Unit(p_ID, TypeUnit::CREATURE, p_CreatureTemplate.m_FactionType)
{
    m_Entry = p_Entry;
    m_MapID = p_MapID;
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    m_SkinID = p_CreatureTemplate.m_SkinID;
    m_Level = p_CreatureTemplate.m_Level;
    m_Name = p_CreatureTemplate.m_Name;
    m_CreatureTemplate = p_CreatureTemplate;

    m_RespawnPosition.SetMapID(p_MapID);
    m_RespawnPosition.SetPosX(p_PosX);
    m_RespawnPosition.SetPosY(p_PosY);
    m_MovementHandler->SetSpeed(0.5f);
    ResetRandMovementTime(false);

	SetPointsSet(PointsSet(m_CreatureTemplate.m_Force, m_CreatureTemplate.m_Stamina, m_CreatureTemplate.m_Dexterity));
    m_RespawnTime = m_CreatureTemplate.m_RespawnTime * IN_MILLISECOND;
}


Creature::~Creature()
{
}

void Creature::RandMoving()
{
    ResetRandMovementTime(true);

    if (m_RandMovementTime <= 0)
        return;

    uint8 l_Orientation = rand() % 4;
    if (GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate.m_MaxRay))
        l_Orientation = GetOrientationToPoint(m_RespawnPosition.GetPosition());
    StartMovement((Orientation)l_Orientation);
}

void Creature::StartMovement(Orientation p_Orientation)
{
    if (m_MovementHandler == nullptr)
        return;

    m_MovementHandler->StartMovement(p_Orientation);
    PacketGoDirection l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID(), GetPosition(), GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Creature::StopMovement()
{
    if (!m_MovementHandler->IsInMovement())
        return;

    m_MovementHandler->StopMovement();
    PacketStopMovement l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID(), GetPosition(), GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Creature::StartAttack(Unit* p_Victim)
{
    m_MovementHandler->StartAttack();
    PacketStartAttack l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID(), GetPosition(), GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Creature::StopAttack()
{
    m_MovementHandler->StopAttack();
    PacketStopAttack l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID());
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Creature::UpdatePassive(sf::Time p_Diff)
{
    if (!IsInCombat())
    {
        while (m_DiffMovementTime > m_RandMovementTime * IN_MICROSECOND) ///< 1000 because microsecond
        {
            if (m_MovementHandler->IsInMovement())
            {
                ResetRandMovementTime(false);
                StopMovement();
            }
            else
                RandMoving();
        }
    }
    else
    {
        if (!IsInMovement()) ///< Go Run
        {
            RandMoving();
            return;
        }

        while (m_DiffMovementTime > m_RandMovementTime * IN_MICROSECOND) ///< 1000 because microsecond
            RandMoving();
    }
}

void Creature::UpdateDefensive(sf::Time p_Diff)
{
    if (!IsInCombat())
    {
        if (m_MovementHandler->IsInAttack() && !m_MovementHandler->IsStopingAttack()) ///< If in combat and still atacking
            StopAttack();

        if (m_MovementHandler->IsInAttack() || m_MovementHandler->IsStopingAttack())
            return;

        while (m_DiffMovementTime > m_RandMovementTime * IN_MICROSECOND) ///< 1000 because microsecond
        {
            if (GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate.m_MaxRay))
            {
                ResetRandMovementTime(true);
                Orientation l_Orientation = GetOrientationToPoint(m_RespawnPosition.GetPosition());
                if (GetOrientation() != l_Orientation || !IsInMovement())
                    StartMovement(l_Orientation);
            }
            else if (m_MovementHandler->IsInMovement())
            {
                OutOfEvade();
                ResetRandMovementTime(false);
                StopMovement();
            }
            else
                RandMoving();
        }
    }
    else
    {
        if (GetAttacker() == nullptr || GetAttacker()->IsDeath())
        {
            OutOfCombat();
            return;
        }

        if (GetVictim() == nullptr)
            SetVictim(GetAttacker());

        if (GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate.m_MaxVision))
        {
            EnterInEvade();
            OutOfCombat();
            return;
        }

        if (GetDistance(GetVictim()) > MELEE_RANGE)
        {
            if (m_MovementHandler->IsInAttack() && !m_MovementHandler->IsStopingAttack())
                StopAttack();
            if (!m_MovementHandler->IsInAttack())
            {
                Orientation l_Orientation = GetOrientationToPoint(GetVictim());
                if (GetOrientation() != l_Orientation || !IsInMovement())
                    StartMovement(l_Orientation);
            }
        }
        else
        {
            if (m_MovementHandler->IsInMovement())
                StopMovement();

            if (m_MovementHandler->IsInAttack() && GetOrientationToPoint(GetVictim()) != GetOrientation())
                UpdateOrientation(GetOrientationToPoint(GetVictim()));

            if (!m_MovementHandler->IsInAttack())
            {
                SetOrientation(GetOrientationToPoint(GetVictim()));
                StartAttack(GetVictim());
            }
        }
    }
}

void Creature::UpdateAgresive(sf::Time p_Diff)
{
    if (!IsInCombat())
    {
        if (m_MovementHandler->IsInAttack() && !m_MovementHandler->IsStopingAttack()) ///< If in combat and still atacking
            StopAttack();

        if (m_MovementHandler->IsInAttack() || m_MovementHandler->IsStopingAttack())
            return;

        Unit* l_Victim = m_Map->GetCloserUnit(this, CaseToPixel(m_CreatureTemplate.m_MaxVision), true, false, true);
        if (GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate.m_MaxRay))
        {            
            if (!IsFollowingPath() && PositionToCasePosition(GetPosition()) != PositionToCasePosition(m_RespawnPosition.GetPosition()))
            {
                m_PathToTargetPosition = m_Map->LaunchPathFinding(PositionToCasePosition(GetPosition()), PositionToCasePosition(m_RespawnPosition.GetPosition()));
            }
            else if (!IsFollowingPath() && PositionToCasePosition(GetPosition()) == PositionToCasePosition(m_RespawnPosition.GetPosition()))
            {
                Orientation l_Orientation = GetOrientationToPoint(m_RespawnPosition.GetPosition());
                if (GetOrientation() != l_Orientation || !IsInMovement())
                    StartMovement(l_Orientation);
            }
           // printf("HERE 0.1 %d %d\n", m_RespawnPosition.GetPosition().m_X, CaseToPixel(m_CreatureTemplate.m_MaxRay));
        }
        else if (m_MovementHandler->IsInMovement())
        {
            OutOfEvade();
            ResetRandMovementTime(false);
            StopMovement();
        }
        else if (l_Victim != nullptr)
        {
            l_Victim->EnterInCombat(this);
            return;
        }
        while (m_DiffMovementTime > m_RandMovementTime * IN_MICROSECOND) ///< 1000 because microsecond
        {
            if (m_MovementHandler->IsInMovement())
            {
                OutOfEvade();
                ResetRandMovementTime(false);
                StopMovement();
            }
            else
                RandMoving();
        }
    }
    else
    {
        if (GetAttacker() == nullptr || GetAttacker()->IsDeath())
        {
            OutOfCombat();
            return;
        }

        if (GetVictim() == nullptr)
            SetVictim(GetAttacker());

        if (GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate.m_MaxVision))
        {
            EnterInEvade();
            OutOfCombat();
            return;
        }

        if (GetDistance(GetVictim()) > MELEE_RANGE)
        {
            if (m_MovementHandler->IsInAttack() && !m_MovementHandler->IsStopingAttack())
                StopAttack();
            if (!m_MovementHandler->IsInAttack())
            {
                m_RandMovementTime = 0;
                if (!IsValidOrientationToPoint((Orientation)GetOrientation(), GetVictim()) || !IsInMovement())
                {
                    Orientation l_Orientation = GetOrientationToPoint(GetVictim());
                    if (GetOrientation() != l_Orientation || !IsInMovement())
                        StartMovement(l_Orientation);
                }
            }
        }
        else
        {
            if (m_MovementHandler->IsInMovement())
                StopMovement();

            if (m_MovementHandler->IsInAttack() && GetOrientationToPoint(GetVictim()) != GetOrientation())
                UpdateOrientation(GetOrientationToPoint(GetVictim()));

            if (!m_MovementHandler->IsInAttack())
            {
                SetOrientation(GetOrientationToPoint(GetVictim()));
                StartAttack(GetVictim());
            }
        }
    }
}

void Creature::UpdateOrientation(Orientation p_Orientation)
{
    m_MovementHandler->SetOrientation(p_Orientation);
    PacketUpdateOrientation l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID(), GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

void Creature::Update(sf::Time p_Diff)
{
    Unit::Update(p_Diff);

    if (!IsInWorld())
        return;

    if (IsFollowingPath())
    {
        Orientation l_Orientation = GetOrientationByPath(m_PathToTargetPosition);
        if (GetOrientation() != l_Orientation || !IsInMovement())
            StartMovement(l_Orientation);
    }
    switch (m_CreatureTemplate.m_AiType)
    {
        case eAiType::PASSIVE:
            UpdatePassive(p_Diff);
            break;
        case eAiType::DEFENSIVE:
            UpdateDefensive(p_Diff);
            break;
        case eAiType::AGRESIVE:
            UpdateAgresive(p_Diff);
            break;
        default:
            break;
    }
    m_DiffMovementTime += p_Diff.asMicroseconds();
}

void Creature::SetHealth(const uint8 & p_Health)
{
    Unit::SetHealth(p_Health);
    if (IsDeath())
        Unspawn();
}

uint32 Creature::GetXpEarn() const
{
    return m_CreatureTemplate.m_Xp;
}

void Creature::ResetRandMovementTime(bool ForMoving)
{
    m_DiffMovementTime = 0;
    if (ForMoving)
    {
        if (m_CreatureTemplate.m_MovingTimeMax - m_CreatureTemplate.m_MovingTimeMin <= 0)
            m_RandMovementTime = m_CreatureTemplate.m_MovingTimeMax;
        else
            m_RandMovementTime = m_CreatureTemplate.m_MovingTimeMin + ((float)(rand() % (uint16)((float)(m_CreatureTemplate.m_MovingTimeMax - m_CreatureTemplate.m_MovingTimeMin) * 100.0f)) / 100.0f);
    }
    else
    {
        if (m_CreatureTemplate.m_StopTimeMax - m_CreatureTemplate.m_StopTimeMin <= 0)
            m_RandMovementTime = m_CreatureTemplate.m_StopTimeMax;
        else
            m_RandMovementTime = m_CreatureTemplate.m_StopTimeMin + ((float)(rand() % (uint16)((float)(m_CreatureTemplate.m_StopTimeMax - m_CreatureTemplate.m_StopTimeMin) * 100.0f)) / 100.0f);
    }
}

void Creature::Unspawn()
{
    m_MovementHandler->StopMovement();
    m_MovementHandler->StopAttack();
    ResetRandMovementTime(false);

    /// Unspawn for players
    PacketUnitRemove l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID());
    m_Map->SendToSet(l_Packet.m_Packet, this);

    SetInWorld(false);
}

void Creature::Respawn()
{
    Unit::Respawn();
    m_DiffMovementTime = 0;
    SetInWorld(true);

    SetHealth(MAX_HEALTH);
    m_ResTimer = 0;

    /// Respawn creature for players
    PacketUnitCreate l_Packet;
    l_Packet.BuildPacket((uint8)TypeUnit::CREATURE, GetID(), GetName(), GetLevel(), GetHealth(), GetSkinID(), GetMapID(), GetPosition(), GetOrientation(), m_MovementHandler->IsInMovement());
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
    while (GetPosX() / TILE_SIZE == p_NextPos.m_X && GetPosY() / TILE_SIZE == p_NextPos.m_Y)
    {
        p_Path.pop_back();
        printf("-------> remove %d id:%d - size : %d\n", p_NextPos.m_X, p_NextPos.m_Y, p_Path.size());

        if (p_Path.empty())
        {
            printf("EMPTY\n", p_NextPos.m_X, p_NextPos.m_Y);
            StopMovement();
            return m_MovementHandler->GetOrientation();
        }
        p_NextPos = p_Path[p_Path.size() - 1];
    }
    return GetOrientationToCase(p_NextPos);
}
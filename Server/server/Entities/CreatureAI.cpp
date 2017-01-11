#include "Creature.hpp"
#include "../Map/Map.hpp"
#include "../World/PacketDefine.hpp"


void Creature::UpdateAI(sf::Time p_Diff)
{
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

        if (GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate.m_MaxRay))
            ReturnToRespawnPoint();
        else if (m_MovementHandler->IsInMovement())
        {
            OutOfEvade();
            ResetRandMovementTime(false);
            StopMovement();
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

        if (GetDistance(m_RespawnPosition.GetPosition()) > CaseToPixel(m_CreatureTemplate.m_MaxRay))
            ReturnToRespawnPoint();
        else if (m_MovementHandler->IsInMovement())
        {
            OutOfEvade();
            ResetRandMovementTime(false);
            StopMovement();
        }
        else if (Unit* l_Victim = m_Map->GetCloserUnit(this, (float)CaseToPixel(m_CreatureTemplate.m_MaxVision), true, false, true))
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
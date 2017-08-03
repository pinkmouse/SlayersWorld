#include "Spell.hpp"
#include "../../World/PacketDefine.hpp"
#include "../../Entities/Player.hpp"
#include "../../World/WorldSocket.hpp"
#include "../../Map/Map.hpp"
#include "../../Map/Case.hpp"


Spell::Spell(SpellTemplate* p_SpellTemplate) :
    m_SpellTemplate(p_SpellTemplate)
{
    m_CastTimer = 0;
    m_SpellEffectsMap[SpellEffectType::Damage] = &Spell::EffectDamage;
    m_SpellEffectsMap[SpellEffectType::Heal] = &Spell::EffectHeal;
    m_SpellEffectsMap[SpellEffectType::ApplyAura] = &Spell::EffectApplyAura;
    m_SpellEffectsMap[SpellEffectType::LearnClass] = &Spell::EffectLearnClass;
    m_SpellEffectsMap[SpellEffectType::LearnSpell] = &Spell::EffectLearnSpell;
    m_SpellEffectsMap[SpellEffectType::Teleport] = &Spell::EffectTeleport;
}

Spell::~Spell()
{
    m_TargetList.clear();
}

SpellTemplate* Spell::GetTemplate() const
{
    return m_SpellTemplate;
}

bool Spell::Prepare(Unit* p_Caster)
{
    m_Caster = p_Caster;

    /// Check Cooldown
    if (p_Caster->HasSpellCooldown(m_SpellTemplate->GetID()))
    {
        if (Player* l_Player = p_Caster->ToPlayer())
        {
            PacketWarningMsg l_Packet;
            l_Packet.BuildPacket(eTypeWarningMsg::Red, "Tant de recharge restant");
            l_Player->GetSession()->send(l_Packet.m_Packet);
        }
        return false;
    }

    /// Check Resources
    std::vector<ResourceNeed>* l_ResourcesNeed = m_SpellTemplate->GetReousrcesNeed();
    for (std::vector<ResourceNeed>::iterator l_It = l_ResourcesNeed->begin(); l_It != l_ResourcesNeed->end(); ++l_It)
    {
        ResourceNeed l_ResourceNedd = (*l_It);
        if (p_Caster->GetResourceNb(l_ResourceNedd.m_ResourceType) < l_ResourceNedd.m_Nb)
        {
            if (Player* l_Player = p_Caster->ToPlayer())
            {
                PacketWarningMsg l_Packet;
                l_Packet.BuildPacket(eTypeWarningMsg::Red, "Resources insuffisantes");
                l_Player->GetSession()->send(l_Packet.m_Packet);
                /// TODO
            }
            return false;
        }
    }

    /// ADD CAST TIME
    SetCastTime((uint64)m_SpellTemplate->GetCastTime() * 1000);
    return true;
}

void Spell::EffectDamage(uint8 p_ID, Unit* p_Target, SpellEffect* p_SpellEffect)
{
    DamageInfo l_DamageInfo = m_Caster->CalculSpellDamageToTarget(p_Target, p_SpellEffect->m_BasePoint1, m_SpellTemplate->GetLevel());
    m_Caster->DealDamage(p_Target, l_DamageInfo);
}

void Spell::EffectApplyAura(uint8 p_ID, Unit* p_Target, SpellEffect* p_SpellEffect)
{
    p_Target->ApplyAuraEffect(p_ID, m_SpellTemplate, m_Caster, (eTypeAuraEffect)p_SpellEffect->m_BasePoint1, p_SpellEffect->m_BasePoint2, p_SpellEffect->m_BasePoint3, p_SpellEffect->m_BasePoint4);
}

void Spell::EffectLearnClass(uint8 p_ID, Unit* p_Target, SpellEffect* p_SpellEffect)
{
    Player* l_Player = p_Target->ToPlayer();
    if (l_Player == nullptr)
        return;

    l_Player->LearnClass((eClass)p_SpellEffect->m_BasePoint1);
}

void Spell::EffectLearnSpell(uint8 p_ID, Unit* p_Target, SpellEffect* p_SpellEffect)
{
    Player* l_Player = p_Target->ToPlayer();
    if (l_Player == nullptr)
        return;

    l_Player->LearnSpell(p_SpellEffect->m_BasePoint1);
}

void Spell::SetCastTime(uint64 p_CastTime)
{
    m_CastTimer = p_CastTime;
}

uint64 Spell::GetCastTime() const
{
    return m_CastTimer;
}

bool Spell::IsReadyToLaunch() const
{
    return m_CastTimer == 0;
}

void Spell::EffectHeal(uint8 p_ID, Unit* p_Target, SpellEffect* p_SpellEffect)
{
    DamageInfo l_DamageInfo = m_Caster->CalculHealToTarget(p_Target, p_SpellEffect->m_BasePoint1, m_SpellTemplate->GetLevel());
    m_Caster->DealHeal(p_Target, l_DamageInfo);
}

void Spell::EffectTeleport(uint8 p_ID, Unit* p_Target, SpellEffect* p_SpellEffect)
{
    if (p_Target->GetMap() == nullptr)
        return;

    int32 l_DistanceCase = p_SpellEffect->m_BasePoint1 / TILE_SIZE;
    int32 l_NbCaseCheck = 0;
    uint32 l_Case = (p_Target->GetPosX() / TILE_SIZE) + ((p_Target->GetPosX() / TILE_SIZE) * p_Target->GetMap()->GetSizeX());
    switch (p_Target->GetOrientation())
    {
    case Orientation::Up :
        for (uint8 i = 0; i < l_DistanceCase; i++)
        {
            if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX(), p_Target->GetPosY() - (TILE_SIZE * i)))
                break;
            l_NbCaseCheck = i;
        }
        break;
    case Orientation::Down :
        for (uint8 i = 0; i < l_DistanceCase; i++)
        {
            if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX(), p_Target->GetPosY() + (TILE_SIZE * i)))
                break;
            l_NbCaseCheck = i;
        }
        break;
    case Orientation::Left :
        for (uint8 i = 0; i < l_DistanceCase; i++)
        {
            if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX() - (TILE_SIZE * i), p_Target->GetPosY()))
                break;
            l_NbCaseCheck = i;
        }
        break;
    case Orientation::Right :
        for (uint8 i = 0; i < l_DistanceCase; i++)
        {
            if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX() + (TILE_SIZE * i), p_Target->GetPosY()))
                break;
            l_NbCaseCheck = i;
        }
        break;
    }
    uint8 l_NbPixel = 0;
    if (l_NbCaseCheck < l_DistanceCase)
    {
        switch (p_Target->GetOrientation())
        {
            case Orientation::Up :
                for (uint8 i = 0; i < TILE_SIZE; i++)
                {
                    if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX(), (uint32)(p_Target->GetPosY() - (uint32)(TILE_SIZE * l_NbCaseCheck) - i)))
                        break;
                    l_NbPixel = i;
                }
                break;
            case Orientation::Down :
            
                for (uint8 i = 0; i < TILE_SIZE; i++)
                {
                    if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX(), (uint32)(p_Target->GetPosY() + (uint32)(TILE_SIZE * l_NbCaseCheck) + i)))
                        break;
                    l_NbPixel = i;
                }
                break;
            case Orientation::Left :
                for (uint8 i = 0; i < TILE_SIZE; i++)
                {
                    if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX() - (uint32)(TILE_SIZE * l_NbCaseCheck) - i, (uint32)(p_Target->GetPosY())))
                        break;
                    l_NbPixel = i;
                }
                break;
            case Orientation::Right :
                for (uint8 i = 0; i < TILE_SIZE; i++)
                {
                    if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX() + (uint32)(TILE_SIZE * l_NbCaseCheck) + i, (uint32)(p_Target->GetPosY())))
                        break;
                    l_NbPixel = i;
                }
                break;
        }
    }
    else
    {
        switch (p_Target->GetOrientation())
        {
        case Orientation::Up:
            for (uint8 i = 0; i < p_SpellEffect->m_BasePoint1 % TILE_SIZE; i++)
            {
                if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX(), (uint32)(p_Target->GetPosY() - (uint32)(TILE_SIZE * l_NbCaseCheck) - i)))
                    break;
                l_NbPixel = i;
            }
            break;
        case Orientation::Down:
            for (uint8 i = 0; i < p_SpellEffect->m_BasePoint1 % TILE_SIZE; i++)
            {
                if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX(), (uint32)(p_Target->GetPosY() + (uint32)(TILE_SIZE * l_NbCaseCheck) + i)))
                    break;
                l_NbPixel = i;
            }
            break;
        case Orientation::Left:
            for (uint8 i = 0; i < p_SpellEffect->m_BasePoint1 % TILE_SIZE; i++)
            {
                if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX() - (uint32)(TILE_SIZE * l_NbCaseCheck) - i, (uint32)(p_Target->GetPosY())))
                    break;
                l_NbPixel = i;
            }
            break;
        case Orientation::Right:
            for (uint8 i = 0; i < p_SpellEffect->m_BasePoint1 % TILE_SIZE; i++)
            {
                if (p_Target->GetMovementHandler()->IsInColision(p_Target->GetPosX() - (uint32)(TILE_SIZE * l_NbCaseCheck) + i, (uint32)(p_Target->GetPosY())))
                    break;
                l_NbPixel = i;
            }
            break;
        }
    }

    /// TELEPORT
    switch (p_Target->GetOrientation())
    {
    case Orientation::Up:
        p_Target->TeleportTo(p_Target->GetPosX(), (uint32)(p_Target->GetPosY() - (uint32)(TILE_SIZE * l_NbCaseCheck) - l_NbPixel), (Orientation)p_Target->GetOrientation());
        break;
    case Orientation::Down:
        p_Target->TeleportTo(p_Target->GetPosX(), (uint32)(p_Target->GetPosY() + (uint32)(TILE_SIZE * l_NbCaseCheck) + l_NbPixel), (Orientation)p_Target->GetOrientation());
        break;
    case Orientation::Left:
        p_Target->TeleportTo(p_Target->GetPosX() - (uint32)(TILE_SIZE * l_NbCaseCheck) - l_NbPixel, (uint32)(p_Target->GetPosY()), (Orientation)p_Target->GetOrientation());
        break;
    case Orientation::Right:
        p_Target->TeleportTo(p_Target->GetPosX() + (uint32)(TILE_SIZE * l_NbCaseCheck) + l_NbPixel, (uint32)(p_Target->GetPosY()), (Orientation)p_Target->GetOrientation());
        break;
    }
}

void Spell::LaunchEffects()
{
    if (m_Caster == nullptr)
        return;

    /// Consume Resources
    std::vector<ResourceNeed>* l_ResourcesNeed = m_SpellTemplate->GetReousrcesNeed();
    for (std::vector<ResourceNeed>::iterator l_It = l_ResourcesNeed->begin(); l_It != l_ResourcesNeed->end(); ++l_It)
    {
        ResourceNeed l_ResourceNedd = (*l_It);
        m_Caster->AddResourceNb(l_ResourceNedd.m_ResourceType, -l_ResourceNedd.m_Nb);
    }

    /// Consume Cooldown
    m_Caster->AddSpellCooldown(m_SpellTemplate->GetID(), m_SpellTemplate->GetCooldown() * 1000 /* MILLISECOND TO MICROSECOND*/);

    std::vector<SpellEffect*>* l_SpellEffects = m_SpellTemplate->GetListEffect();
    uint8 l_SpellEffectID = 0;
    for (std::vector<SpellEffect*>::iterator l_It = l_SpellEffects->begin(); l_It != l_SpellEffects->end(); ++l_It)
    {
        SpellEffect* l_SpellEffect = (*l_It);
        std::vector<Unit*> l_Targets = SearchTargets(l_SpellEffect->m_Target, l_SpellEffect->m_RadiusMax, l_SpellEffect->m_RadiusMin);

        m_SpellFunc l_Fun = m_SpellEffectsMap[l_SpellEffect->m_EffectID];
        if (l_Fun != nullptr)
        {
            for (uint8 i = 0; i < l_Targets.size(); ++i)
            {
                /// Start Visual on target, aura visual are launching since Aura class
                if (m_SpellTemplate->GetVisualIDTarget() >= 0 && m_SpellTemplate->GetDuration() == 0)
                {
                    PacketUnitPlayVisual l_Packet;
                    l_Packet.BuildPacket(l_Targets[i]->GetType(), l_Targets[i]->GetID(), false, (uint8)m_SpellTemplate->GetVisualIDTarget());
                    l_Targets[i]->GetMap()->SendToSet(l_Packet.m_Packet, l_Targets[i]);
                }
                if (m_SpellTemplate->GetVisualIDTargetUnder() >= 0 && m_SpellTemplate->GetDuration() == 0)
                {
                    PacketUnitPlayVisual l_Packet;
                    l_Packet.BuildPacket(l_Targets[i]->GetType(), l_Targets[i]->GetID(), true, (uint8)m_SpellTemplate->GetVisualIDTargetUnder());
                    l_Targets[i]->GetMap()->SendToSet(l_Packet.m_Packet, l_Targets[i]);
                }

                (this->*(l_Fun))(l_SpellEffectID, l_Targets[i], l_SpellEffect);
            }
        }
        else
            printf("Spell type: %d Unknow\n", l_SpellEffect->m_EffectID);
        l_SpellEffectID++;
    }

    delete this;
}

std::vector<Unit*> Spell::SearchTargets(SpellTarget p_TargetType, float p_RadiusMax, float p_RadiusMin)
{
    if (!m_TargetList.empty())
        return m_TargetList;

    std::vector<Unit*> l_Targets;

    switch (p_TargetType)
    {
    case SpellTarget::Caster:
        l_Targets.push_back(m_Caster);
        break;
    case SpellTarget::CloserEnemy:
    {
        Unit* l_Target = m_Caster->GetMap()->GetCloserUnit(m_Caster, p_RadiusMax, true, true, true);
        if (l_Target != nullptr)
            l_Targets.push_back(l_Target);
        break;
    }
    case SpellTarget::ZoneEnemy:
    {
        l_Targets = m_Caster->GetMap()->GetUnitsInRadius(m_Caster, p_RadiusMin, p_RadiusMax, true, true);
        break;
    }
    case SpellTarget::ZoneAlly:
    {
        l_Targets = m_Caster->GetMap()->GetUnitsInRadius(m_Caster, p_RadiusMin, p_RadiusMax, true, false);
        break;
    }
    default:
        break;
    }
    return l_Targets;
}

void Spell::Interrupt()
{
    delete this;
}

void Spell::SetTargetList(std::vector<Unit*> p_TargetList)
{
    m_TargetList = p_TargetList;
}
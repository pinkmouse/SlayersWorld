#include "GameObject.hpp"
#include "../System/Required/Required.hpp"
#include "../Map/Map.hpp"


GameObjectTemplate::GameObjectTemplate() :
    m_Id(0),
    m_Type(eGameObjectTemplate::GameObjectSpellLauncher),
    m_Duration(-1),
    m_RespawnTime(-1)
{
}

GameObjectTemplate::GameObjectTemplate(uint16 p_Id, int32 p_Duration, int32 p_RespawnTime, eGameObjectTemplate p_Type, int16 p_SkinID, bool p_Blocking, Required* p_Required) :
    m_Id(p_Id),
    m_Type(p_Type),
    m_SkinID(p_SkinID),
    m_Duration(p_Duration),
    m_RespawnTime(p_RespawnTime * IN_MILLISECOND),
    m_Blocking(p_Blocking),
    m_Required(p_Required)
{
}

GameObjectTemplate::~GameObjectTemplate()
{
}

uint16 GameObjectTemplate::GetID() const
{
    return m_Id;
}

void GameObjectTemplate::SetData(uint8 p_ID, uint32 p_Data)
{
    m_Datas[p_ID] = p_Data;
}

uint32 GameObjectTemplate::GetData(uint8 p_ID)
{
    if (m_Datas.find(p_ID) == m_Datas.end())
        return 0;
    return  m_Datas[p_ID];
}

int32 GameObjectTemplate::GetDuration() const
{
    return m_Duration;
}

int32 GameObjectTemplate::GetRespawnTime() const
{
    return m_RespawnTime;
}

int16 GameObjectTemplate::GetSkinID() const
{
    return m_SkinID;
}

bool GameObjectTemplate::GetBlocking() const
{
    return m_Blocking;
}

Required* GameObjectTemplate::GetRequired()
{
    return m_Required;
}

eGameObjectTemplate GameObjectTemplate::GetType() const
{
    return m_Type;
}

GameObject::GameObject(uint16 p_Id,  Map* p_Map, uint32 p_PosX, uint32 p_PosY, GameObjectTemplate* p_GobTemplate) :
    DynamicObject(p_Id, p_GobTemplate->GetID(), TypeUnit::GAMEOBJECT, p_Map, p_PosX, p_PosY, p_GobTemplate->GetSkinID()),
    m_GobTemplate(p_GobTemplate)
{
    m_RespawnTime = p_GobTemplate->GetRespawnTime();

    switch (m_GobTemplate->GetType())
    {
    case eGameObjectTemplate::GameObjectSpellLauncher:
    {
        AddSpellID(m_GobTemplate->GetData(0), 0);
        break;
    }
    default:
        break;
    }
}

GameObject::~GameObject()
{
}

void GameObject::Update(sf::Time m_Diff)
{
    Unit::Update(m_Diff);
    switch (m_GobTemplate->GetType())
    {
        case eGameObjectTemplate::GameObjectSpellLauncher :
        {
            if (m_GobTemplate->GetData(0) < 0) ///< No Spells
                break;

            if (HasSpellCooldown(m_GobTemplate->GetData(0)))
                return;

            CastSpell(m_GobTemplate->GetData(0));
            break;
        }
        default:
            break;
    }
}

bool GameObject::IsBlocking() const
{
    return m_GobTemplate->GetBlocking();
}

void GameObject::UnitEnterInCase(Unit* p_Unit)
{
    if (!IsInWorld())
        return;

    switch (m_GobTemplate->GetType())
    {
        case eGameObjectTemplate::GameObjectTrap:
        {
            if (!p_Unit->CanBeTarget())
                return;

            bool l_Launch = GetMap()->LauchTrapHandle(this, p_Unit);

            if (!l_Launch)
                break;

            SetResourceNb(eResourceType::Health, 0);
            if (m_GobTemplate->GetData(0) < 0) ///< No Spells
                break;
            std::vector<Unit*> l_TargetList;
            l_TargetList.push_back(p_Unit);
            CastSpell(m_GobTemplate->GetData(0), l_TargetList);
            break;
        }
    default:
        break;
    }
}

void GameObject::UnitAction(Unit* p_Unit)
{
}

bool GameObject::CanBeWalk()
{
    return true;
}

GameObjectTemplate* GameObject::GetGameObjectTemplate() const
{
    return m_GobTemplate;
}

void GameObject::ActionFrom(Player* p_Player)
{
    Unit::ActionFrom(p_Player);

    printf("Enter %d\n", m_GobTemplate->GetType());

    switch (m_GobTemplate->GetType())
    {
    case eGameObjectTemplate::GameObjectQuest:
        if (m_GobTemplate->GetRequired() != nullptr && m_GobTemplate->GetRequired()->IsValid(p_Player))
        {
            if (p_Player->CheckQuestObjective(eObjectifType::RecoltGob, m_GobTemplate->GetID()))
                SetResourceNb(eResourceType::Health, 0);
        }
        break;
    case eGameObjectTemplate::GameObjectDoor:
        if (m_GobTemplate->GetRequired() != nullptr && m_GobTemplate->GetRequired()->IsValid(p_Player))
                SetResourceNb(eResourceType::Health, 0);
        printf("Enter %d\n", m_GobTemplate->GetType());

        break;
    case eGameObjectTemplate::GameObjectLaunchGossip:
        if ((m_GobTemplate->GetRequired() != nullptr && m_GobTemplate->GetRequired()->IsValid(p_Player)) || m_GobTemplate->GetRequired() == nullptr)
        {
            GossipTo(p_Player);
        }
        break;
    default:
        break;
    }
}
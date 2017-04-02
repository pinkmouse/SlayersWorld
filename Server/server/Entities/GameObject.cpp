#include "GameObject.hpp"


GameObjectTemplate::GameObjectTemplate() :
    m_Id(0),
    m_Type(eGameObjectTemplate::GameObjectSpellLauncher),
    m_Duration(-1),
    m_RespawnTime(-1)
{
}

GameObjectTemplate::GameObjectTemplate(uint16 p_Id, int32 p_Duration, int32 p_RespawnTime, eGameObjectTemplate p_Type, int16 p_SkinID) :
    m_Id(p_Id),
    m_Type(p_Type),
    m_SkinID(p_SkinID),
    m_Duration(p_Duration),
    m_RespawnTime(p_RespawnTime)
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


eGameObjectTemplate GameObjectTemplate::GetType() const
{
    return m_Type;
}

GameObject::GameObject(uint16 p_Id,  Map* p_Map, uint32 p_PosX, uint32 p_PosY, GameObjectTemplate* p_GobTemplate) :
    DynamicObject(p_Id, p_GobTemplate->GetID(), TypeUnit::GAMEOBJECT, p_Map, p_PosX, p_PosY, p_GobTemplate->GetSkinID()),
    m_GobTemplate(p_GobTemplate)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(sf::Time m_Diff)
{
}

bool GameObject::IsBlocking()
{
    return false;
}


void GameObject::UnitEnterInCase(Unit* p_Unit)
{
}

bool GameObject::CanBeWalk()
{
    return true;
}
#include "DynamicObjectManager.hpp"


DynamicObjectManager::~DynamicObjectManager()
{
}

DynamicObjectManager::DynamicObjectManager()
{
}

void DynamicObjectManager::AddAreatriggerTemplate(AreatriggerTemplate p_AreatriggerTemplace)
{
    m_AreatriggerList[p_AreatriggerTemplace.GetID()] = p_AreatriggerTemplace;
}

AreatriggerTemplate* DynamicObjectManager::GetAreatriggerTemplate(uint16 p_Id)
{
    if (m_AreatriggerList.find(p_Id) == m_AreatriggerList.end())
        return nullptr;

    return &m_AreatriggerList[p_Id];
}

void DynamicObjectManager::AddGameObjectTemplate(GameObjectTemplate p_GameObjectTemplate)
{
    m_GobList[p_GameObjectTemplate.GetID()] = p_GameObjectTemplate;
}

GameObjectTemplate* DynamicObjectManager::GetGameObjectTemplate(uint16 p_Id)
{
    if (m_GobList.find(p_Id) == m_GobList.end())
        return nullptr;

    return &m_GobList[p_Id];
}
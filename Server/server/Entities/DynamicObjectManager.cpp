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

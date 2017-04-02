#pragma once
#include "../Define.hpp"
#include "Areatrigger.hpp"
#include "GameObject.hpp"

class DynamicObjectManager
{
public:
    ~DynamicObjectManager();
    DynamicObjectManager();
    void AddAreatriggerTemplate(AreatriggerTemplate);
    AreatriggerTemplate* GetAreatriggerTemplate(uint16);
    void AddGameObjectTemplate(GameObjectTemplate);
    GameObjectTemplate* GetGameObjectTemplate(uint16);

private:
    std::map<uint16, AreatriggerTemplate> m_AreatriggerList;
    std::map<uint16, GameObjectTemplate> m_GobList;
};


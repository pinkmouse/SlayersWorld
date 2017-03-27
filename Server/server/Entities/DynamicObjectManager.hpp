#pragma once
#include "../Define.hpp"
#include "Areatrigger.hpp"

class DynamicObjectManager
{
public:
    ~DynamicObjectManager();
    DynamicObjectManager();
    void AddAreatriggerTemplate(AreatriggerTemplate);
    AreatriggerTemplate* GetAreatriggerTemplate(uint16);

private:
    std::map<uint16, AreatriggerTemplate> m_AreatriggerList;
};


#pragma once
#include "DynamicObject.hpp"
#include "../Define.hpp"

class Areatrigger : public DynamicObject
{
public:
    Areatrigger(Case*, eAreatriggerType);
    ~Areatrigger();
    void Update(sf::Time);
    void SetData(uint8, uint32);
    uint32 GetData(uint8);

private:
    std::map<uint8, uint32> m_Datas;
    eAreatriggerType m_Type;
};


#pragma once
#include "DynamicObject.hpp"
#include "../Define.hpp"

class AreatriggerTemplate
{
public:
    AreatriggerTemplate();
    AreatriggerTemplate(uint16, float, eAreatriggerType, int16);
    ~AreatriggerTemplate();
    void SetData(uint8, uint32);
    uint32 GetData(uint8);
    float GetRadius() const;
    int16 GetSkinID() const;
    eAreatriggerType GetType() const;
    uint16 GetID() const;

private:
    uint16 m_Id;
    std::map<uint8, uint32> m_Datas;
    eAreatriggerType m_Type;
    int16 m_SkinID;
    float m_Radius;
};

class Areatrigger : public DynamicObject
{
public:
    Areatrigger(uint16, Map*, uint32, uint32, AreatriggerTemplate*);
    ~Areatrigger();
    void Update(sf::Time);
    void UnitEnterInCase(Unit*);
    bool CanBeWalk();
    bool IsBlocking() const;
    void ActionFrom(Player*);

private:
    AreatriggerTemplate* m_AreatriggerTemplate;
    uint64 m_Timer;
};


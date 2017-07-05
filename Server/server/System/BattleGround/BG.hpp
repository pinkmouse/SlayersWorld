#pragma once
#include "../../Define.hpp"
#include "../../Map/Map.hpp"

class BG
{
public:
    BG(TypeBG, uint16);
    ~BG();
    virtual void Update(sf::Time);
    void AddUnit(Unit*);
    void RemoveUnit(Unit*);

private:
    TypeBG  m_Type;
    Map* m_Map;
};
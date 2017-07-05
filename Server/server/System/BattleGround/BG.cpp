#include "BG.hpp"


BG::BG(TypeBG p_TypeBG, uint16 p_MapID) :
    m_Type(p_TypeBG)
{
    m_Map = new Map(p_MapID);
    if (!m_Map->InitializeMap("map" + std::to_string(p_MapID) + ".map"))
        printf("Error map %d\n don't exist for BG", p_MapID);
}

BG::~BG()
{
}

void BG::Update(sf::Time p_Time)
{
    m_Map->Update(p_Time);
}

void BG::AddUnit(Unit* p_Player)
{
    m_Map->AddUnit(p_Player);
}

void BG::RemoveUnit(Unit* p_Player)
{
    m_Map->RemoveUnit(p_Player);
}
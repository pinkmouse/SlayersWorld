#include "Square.hpp"


Square::Square()
{
}


Square::~Square()
{
}

void Square::AddUnit(Unit* p_Unit)
{
    std::vector<Unit*>::iterator l_It = std::find(m_ListUnit.begin(), m_ListUnit.end(), p_Unit);
    if (l_It == m_ListUnit.end())
        m_ListUnit.push_back(p_Unit);
}

void Square::RemoveUnit(Unit* p_Unit)
{
    std::vector<Unit*>::iterator l_It = std::find(m_ListUnit.begin(), m_ListUnit.end(), p_Unit);
    if (l_It != m_ListUnit.end())
        m_ListUnit.erase(l_It);
}

std::vector<Unit*>* Square::GetList()
{
    return &m_ListUnit;
}
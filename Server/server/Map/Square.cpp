#include "Square.hpp"


Square::Square()
{
}


Square::~Square()
{
}

void Square::AddUnit(Unit* p_Unit)
{
    m_ListUnitSquare[p_Unit->GetType()][p_Unit->GetID()] = p_Unit;
}

void Square::RemoveUnit(Unit* p_Unit)
{
    m_ListUnitSquare[p_Unit->GetType()].erase(p_Unit->GetID());
}

std::map<TypeUnit, std::map<uint16, Unit*>>* Square::GetList()
{
    return &m_ListUnitSquare;
}

std::map<uint16, Unit*>* Square::GetList(TypeUnit p_TypeID)
{
    return &m_ListUnitSquare[p_TypeID];
}

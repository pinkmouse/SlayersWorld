#pragma once

#include <map>
#include "../Entities/Unit.hpp"

class Square
{
public:
    Square();
    ~Square();
    void AddUnit(Unit*);
    void RemoveUnit(Unit*);
    std::map<TypeUnit, std::map<uint16, Unit*>>* GetList();
    std::map<uint16, Unit*>* GetList(TypeUnit);

private:
    std::map<TypeUnit, std::map<uint16, Unit*>> m_ListUnitSquare;
};


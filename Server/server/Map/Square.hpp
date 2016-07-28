#pragma once

#include <vector>
#include "../Entities/Unit.hpp"

class Square
{
public:
    Square();
    ~Square();
    void AddUnit(Unit*);
    void RemoveUnit(Unit*);
    std::vector<Unit*>* GetList();

private:
    std::vector<Unit*> m_ListUnit;
};


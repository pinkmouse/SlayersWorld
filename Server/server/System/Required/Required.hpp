#pragma once
#include "../../Define.hpp"
#include "../../Entities/Player.hpp"
#include "SubRequired.hpp"

class Required
{
public:
    Required();
    ~Required();
    void AddSubRequired(SubRequired*);
    bool IsValid(const Player*) const;

private:
    uint16 m_Id;
    std::vector< SubRequired* > m_SubRequiredList;
};
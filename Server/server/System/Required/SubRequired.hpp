#pragma once
#include "../../Define.hpp"
#include "../../Entities/Player.hpp"


class SubRequired
{
public:
    SubRequired();
    ~SubRequired();
    virtual bool IsValid(const Player*) const;

private:
    uint16 m_Id;
    eRequiredType m_RequiredType;
};
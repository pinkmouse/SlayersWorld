#pragma once
#include "../../Define.hpp"
#include "../../Entities/Player.hpp"
#include "Required.hpp"

class RequiredManager
{
public:
    RequiredManager();
    ~RequiredManager();
    void AddSubRequiered(uint16, eRequiredType, uint32, uint32);
    void AddSubRequiered(uint16, SubRequired*);
    Required* GetRequiered(uint16);

private:
    std::map< uint16, Required > m_RequiredList;
};
#pragma once
#include "../../Define.hpp"
#include "Resource.hpp"

class ResourceHealth : public Resource
{
public:
    ResourceHealth();
    ~ResourceHealth();

    void Update(sf::Time);
    void SetNumber(uint8);
    void AddNumber(int8);

    ResourceHealth& operator+(uint8 const& p_Nb)
    { 
        m_Number += p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_HEALTH);
        return *this;
    }

    ResourceHealth& operator+=(uint8 const& p_Nb)
    { 
        m_Number += p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_HEALTH);
        return *this;
    }

    ResourceHealth& operator=(uint8 const& p_Nb)
    {
        m_Number = p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_HEALTH);
        return *this;
    }
};
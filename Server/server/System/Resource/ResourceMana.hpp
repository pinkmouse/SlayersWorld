#pragma once
#include "../../Define.hpp"
#include "Resource.hpp"

class ResourceMana : public Resource
{
public:
    ResourceMana();
    ~ResourceMana();

    void Update(sf::Time);
    void SetNumber(uint8);
    void AddNumber(int8);

    ResourceMana& operator+(uint8 const& p_Nb)
    { 
        m_Number += p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_HEALTH);
        return *this;
    }

    ResourceMana& operator+=(uint8 const& p_Nb)
    { 
        m_Number += p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_HEALTH);
        return *this;
    }

    ResourceMana& operator=(uint8 const& p_Nb)
    {
        m_Number = p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_HEALTH);
        return *this;
    }
};
#pragma once
#include "../../Define.hpp"
#include "Resource.hpp"

class ResourceAlignment : public Resource
{
public:
    ResourceAlignment();
    ~ResourceAlignment();

    void Update(sf::Time);
    void SetNumber(uint8);
    void AddNumber(int8);

    ResourceAlignment& operator+(uint8 const& p_Nb)
    { 
        m_Number += p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_ALIGNMENT);
        return *this;
    }

    ResourceAlignment& operator+=(uint8 const& p_Nb)
    { 
        m_Number += p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_ALIGNMENT);
        return *this;
    }

    ResourceAlignment& operator=(uint8 const& p_Nb)
    {
        m_Number = p_Nb;
        m_Number = std::min(m_Number, (uint8)MAX_ALIGNMENT);
        return *this;
    }
};
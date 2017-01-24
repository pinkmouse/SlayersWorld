#pragma once
#include "../../Define.hpp"
#include <algorithm>
#include <SFML/System/Time.hpp>

class Resource
{
public:
    Resource(eResourceType);
    Resource();
    ~Resource();
    eResourceType GetType() const;
    uint8 GetNumber() const;
    void SetNumber(uint8);
    void AddNumber(int8);
    //virtual void Update(sf::Time);

    Resource& operator+(uint8 const& p_Nb) 
    {
        m_Number += p_Nb;
        return *this;
    }

    Resource& operator+=(uint8 const& p_Nb) 
    {
        m_Number += p_Nb;
        return *this; 
    }

    Resource& operator=(uint8 const& p_Nb)
    {
        m_Number = p_Nb;
        return *this;
    }

protected:
    eResourceType   m_Type;
    uint64 m_DifTime;
    uint8 m_Number;
};


#include "Resource.hpp"

Resource::Resource(eResourceType p_Type) :
    m_Type(p_Type),
    m_DifTime(0),
    m_Number(0)
{
}

Resource::Resource() :
    m_Type(eResourceType::None),
    m_DifTime(0),
    m_Number(0)
{
}

Resource::~Resource()
{
}

eResourceType Resource::GetType() const
{
    return m_Type;
}

void Resource::SetNumber(uint8 p_Number)
{
    m_Number = p_Number;
}

void Resource::AddNumber(int8 p_Number)
{
    int8 l_Nb = m_Number;
    if (l_Nb + p_Number < 0)
        m_Number = 0;
    else
        m_Number += p_Number;
}

uint8 Resource::GetNumber() const
{
    return m_Number;
}

void Resource::Update(sf::Time)
{
}
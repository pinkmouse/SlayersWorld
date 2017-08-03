#include "ResourceMana.hpp"

ResourceMana::ResourceMana() :
    Resource(eResourceType::Mana)
{
    m_Number = (uint8)MAX_MANA;
}

ResourceMana::~ResourceMana()
{
}

void ResourceMana::SetNumber(uint8 m_Nb)
{
    m_Number = m_Nb;
    m_Number = std::min(m_Number, (uint8)MAX_MANA);
}

void ResourceMana::AddNumber(int8 p_Number)
{
    int8 l_Nb = m_Number;
    if (l_Nb + p_Number < 0)
        m_Number = 0;
    else
        m_Number += p_Number;
    m_Number = std::min(m_Number, (uint8)MAX_MANA);
}

void ResourceMana::Update(sf::Time p_DifTime)
{
    m_DifTime += p_DifTime.asMicroseconds();
    if (m_DifTime >= REGEN_HEALTH_TIMER * IN_MILLISECOND)
    {
        AddNumber(10);
        m_DifTime -= REGEN_HEALTH_TIMER * IN_MILLISECOND;
    }
}

#include "ResourceHealth.hpp"

ResourceHealth::ResourceHealth() :
    Resource(eResourceType::Health)
{
    m_Number = (uint8)MAX_HEALTH;
}

ResourceHealth::~ResourceHealth()
{
}

void ResourceHealth::SetNumber(uint8 m_Nb)
{
    m_Number = m_Nb;
    m_Number = std::min(m_Number, (uint8)MAX_HEALTH);
}

void ResourceHealth::AddNumber(int8 p_Number)
{
    int8 l_Nb = m_Number;
    if (l_Nb + p_Number < 0)
        m_Number = 0;
    else
        m_Number += p_Number;
    m_Number = std::min(m_Number, (uint8)MAX_HEALTH);
}

void ResourceHealth::Update(sf::Time p_DifTime)
{
    m_DifTime += p_DifTime.asMicroseconds();
    if (m_DifTime >= REGEN_HEALTH_TIMER * IN_MILLISECOND)
    {
        AddNumber(5);
        m_DifTime -= REGEN_HEALTH_TIMER * IN_MILLISECOND;
    }
}

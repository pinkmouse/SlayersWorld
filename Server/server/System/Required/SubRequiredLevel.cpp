#include "SubRequiredLevel.hpp"

SubRequiredLevelMin::SubRequiredLevelMin(uint8 p_Level) :
    m_Level(p_Level)
{
}

SubRequiredLevelMin::~SubRequiredLevelMin()
{
}

bool SubRequiredLevelMin::IsValid(const Player* p_Player) const
{
    if (p_Player->GetLevel() < m_Level)
        return false;
    return true;
}

SubRequiredLevelMax::SubRequiredLevelMax(uint8 p_Level) :
    m_Level(p_Level)
{
}

SubRequiredLevelMax::~SubRequiredLevelMax()
{
}

bool SubRequiredLevelMax::IsValid(const Player* p_Player) const
{
    if (p_Player->GetLevel() > m_Level)
        return false;
    return true;
}


SubRequiredAccessLevel::SubRequiredAccessLevel(uint16 p_AccessLevel, uint16) :
    m_AccessLevel(p_AccessLevel)
{
}

SubRequiredAccessLevel::~SubRequiredAccessLevel()
{
}

bool SubRequiredAccessLevel::IsValid(const Player* p_Player) const
{
    if (p_Player->GetAccessType() >= m_AccessLevel)
        return true;
    return false;
}

SubRequiredClass::SubRequiredClass(uint8 p_Class) :
    m_Class(p_Class)
{
}

SubRequiredClass::~SubRequiredClass()
{
}

bool SubRequiredClass::IsValid(const Player* p_Player) const
{
    if (p_Player->GetClass() == (eClass)m_Class)
        return true;
    return false;
}


SubRequiredHasSpell::SubRequiredHasSpell(uint16 p_SpellID) :
    m_Spell(p_SpellID)
{
}

SubRequiredHasSpell::~SubRequiredHasSpell()
{
}

bool SubRequiredHasSpell::IsValid(const Player* p_Player) const
{
    if (p_Player->HasSpell(m_Spell))
        return true;
    return false;
}

SubRequiredHasNotSpell::SubRequiredHasNotSpell(uint16 p_SpellID) :
    m_Spell(p_SpellID)
{
}

SubRequiredHasNotSpell::~SubRequiredHasNotSpell()
{
}

bool SubRequiredHasNotSpell::IsValid(const Player* p_Player) const
{
    if (!p_Player->HasSpell(m_Spell))
        return true;
    return false;
}

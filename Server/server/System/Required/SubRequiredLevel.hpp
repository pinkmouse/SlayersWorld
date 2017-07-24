#pragma once
#include "../../Define.hpp"
#include "../../Entities/Player.hpp"
#include "SubRequired.hpp"

class SubRequiredLevelMin : public SubRequired
{
public:
    SubRequiredLevelMin(uint8);
    ~SubRequiredLevelMin();
    bool IsValid(const Player*) const;

private:
    uint8 m_Level;
};

class SubRequiredLevelMax : public SubRequired
{
public:
    SubRequiredLevelMax(uint8);
    ~SubRequiredLevelMax();
    bool IsValid(const Player*) const;

private:
    uint8 m_Level;
};


class SubRequiredAccessLevel : public SubRequired
{
public:
    SubRequiredAccessLevel(uint16, uint16);
    ~SubRequiredAccessLevel();
    bool IsValid(const Player*) const;

private:
    uint16 m_AccessLevel;
};

class SubRequiredClass : public SubRequired
{
public:
    SubRequiredClass(uint8);
    ~SubRequiredClass();
    bool IsValid(const Player*) const;

private:
    uint8 m_Class;
};

class SubRequiredHasSpell : public SubRequired
{
public:
    SubRequiredHasSpell(uint16);
    ~SubRequiredHasSpell();
    bool IsValid(const Player*) const;

private:
    uint16 m_Spell;
};

class SubRequiredHasNotSpell : public SubRequired
{
public:
    SubRequiredHasNotSpell(uint16);
    ~SubRequiredHasNotSpell();
    bool IsValid(const Player*) const;

private:
    uint16 m_Spell;
};
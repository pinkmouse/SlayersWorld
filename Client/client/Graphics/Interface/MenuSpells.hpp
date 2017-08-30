#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class SubMenuSpells : public Menu
{
public:
    SubMenuSpells();
    ~SubMenuSpells();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void SetCurrentSpell(const uint16 &);

private:
    uint16 m_CurrentSpell;
};

class MenuSpells : public Menu
{
public:
    MenuSpells();
    ~MenuSpells();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void AddSpell(const uint16 &, const std::string &);
    void RemoveTitle(const uint16 &);

private:
    std::map<uint16, std::string>   m_Spells;
    SubMenuSpells                   m_SubMenu;
};
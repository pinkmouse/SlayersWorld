#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class SubMenuEquipment : public Menu
{
public:
    SubMenuEquipment();
    ~SubMenuEquipment();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void SetCurrentItem(const Item &, const eTypeEquipment &);

private:
    std::pair<Item, eTypeEquipment> m_CurrentItem;
};

class MenuEquipment : public Menu
{
public:
    MenuEquipment();
    ~MenuEquipment();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void AddEquipment(const eTypeEquipment &, const Item &);
    void RemoveEquipment(const eTypeEquipment &);

private:
    std::map<eTypeEquipment, Item>  m_Equipment;
    SubMenuEquipment                m_SubMenu;
};
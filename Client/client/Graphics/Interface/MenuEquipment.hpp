#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class MenuEquipment : public Menu
{
public:
    MenuEquipment();
    ~MenuEquipment();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void AddEquipment(const eTypeEquipment &, const std::string &);
    void RemoveTitle(const uint16 &);

private:
    std::map<eTypeEquipment, std::string> m_Equipment;
};
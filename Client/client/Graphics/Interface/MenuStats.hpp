#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class MenuStats : public Menu
{
public:
    MenuStats();
    ~MenuStats();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);

private:

};
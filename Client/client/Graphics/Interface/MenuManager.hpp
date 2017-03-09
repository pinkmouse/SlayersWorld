#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"
#include <vector>

class MenuManager : public Menu
{
public:
    MenuManager();
    ~MenuManager();
    void KeyPress(const sf::Keyboard::Key &);
    std::vector<Menu*> GetOpenMenus();
    void OpenMenu(const uint16 &);

private:
    std::map<eMenuType, Menu> m_ListMenu;
};
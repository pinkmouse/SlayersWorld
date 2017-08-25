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
    void GenericAction(const uint16 &);
    void AddElementToMenu(eMenuType, const uint8 &, const uint8 &, const std::string &);
    Menu* GetMenu(eMenuType);

private:
    std::map<eMenuType, Menu*> m_ListMenu;
};
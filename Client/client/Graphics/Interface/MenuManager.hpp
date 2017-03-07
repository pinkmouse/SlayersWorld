#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"
#include <vector>

class MenuManager : public Menu
{
public:
    MenuManager();
    ~MenuManager();
    void KeyPress(const eKeyBoardAction &);
    std::vector<Menu*> GetOpenMenus();

private:
    std::map<eMenuType, Menu> m_ListMenu;
};
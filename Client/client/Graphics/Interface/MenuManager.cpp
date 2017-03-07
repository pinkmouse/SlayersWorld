#include "MenuManager.hpp"
#include "MenuMain.hpp"
#include "../../Global.hpp"

MenuManager::MenuManager() :
    Menu(1, 6)
{
    m_Pos.x = 30;
    m_Pos.y = 30;
    AddElement(0, 0, "Quests");
    AddElement(0, 1, "Save");
    AddElement(0, 2, "Escape");
    SetSelectedElement(0, 0);
}

MenuManager::~MenuManager()
{
}

void MenuManager::KeyPress(const eKeyBoardAction & p_Key)
{
}

std::vector<Menu*> MenuManager::GetOpenMenus()
{
    std::vector<Menu*> l_Result;
    if (!IsOpen()) ///< Manager himslef
        return l_Result;

    l_Result.push_back(this);
    for (std::map<eMenuType, Menu>::iterator l_It = m_ListMenu.begin(); l_It != m_ListMenu.end(); ++l_It)
    {
        if ((*l_It).second.IsOpen())
            l_Result.push_back(&(*l_It).second);
    }
    return l_Result;
}
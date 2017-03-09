#include "MenuManager.hpp"
#include "MenuQuest.hpp"
#include "../../Global.hpp"

MenuManager::MenuManager() :
    Menu(1, 6)
{
    m_Pos.x = 10;
    m_Pos.y = 30;
    AddElement(0, 0, "Quests");
    GetElement(0, 0)->SetFunc(&Menu::OpenMenu, 0);
    AddElement(0, 1, "Save");
    GetElement(0, 1)->SetFunc(&Menu::OpenMenu, 1);
    AddElement(0, 5, "Escape");
    GetElement(0, 5)->SetFunc(&Menu::OpenMenu, 2);
    SetSelectedElement(0, 0);

    m_ListMenu[eMenuType::QuestMenu] = MenuQuest();
}

MenuManager::~MenuManager()
{
}

void MenuManager::OpenMenu(const uint16 & p_MenuID)
{

    switch (p_MenuID)
    {
        case 0:
            m_ListMenu[eMenuType::QuestMenu].Open();
            break;
        case 5:
            Close();
            break;
    }
}

void MenuManager::KeyPress(const sf::Keyboard::Key & p_Key)
{
    switch (p_Key)
    {
        case sf::Keyboard::Return:
        {
            std::pair<uint8, uint8> l_SelectedElem = GetSelectedElement();
            MenuElement* l_Elem = GetElement(l_SelectedElem.first, l_SelectedElem.second);
            if (l_Elem == nullptr)
                break;
            l_Elem->LaunchFunc(l_SelectedElem.second, this);
            break;
        }
        case sf::Keyboard::Up :
        {
            SelectNextElementOn(Orientation::Up);
            break;
        }
        case sf::Keyboard::Down:
        {
            SelectNextElementOn(Orientation::Down);
            break;
        }
    }
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
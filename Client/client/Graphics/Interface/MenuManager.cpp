#include "MenuManager.hpp"
#include "MenuQuest.hpp"
#include "MenuTitles.hpp"
#include "MenuWardrobe.hpp"
#include "MenuStats.hpp"
#include "../../Global.hpp"

MenuManager::MenuManager() :
    Menu(1, 6)
{
    m_Pos.x = 10;
    m_Pos.y = 30;
    AddElement(0, 0, "Stats");
    GetElement(0, 0)->SetFunc(&Menu::GenericAction, 0);
    AddElement(0, 1, "Title");
    GetElement(0, 1)->SetFunc(&Menu::GenericAction, 1);
    AddElement(0, 2, "Wardrobe");
    GetElement(0, 2)->SetFunc(&Menu::GenericAction, 2);
    AddElement(0, 3, "Save");
    GetElement(0, 3)->SetFunc(&Menu::GenericAction, 3);
    AddElement(0, 5, "Escape");
    GetElement(0, 5)->SetFunc(&Menu::GenericAction, 5);
    SetSelectedElement(0, 0);

    m_ListMenu[eMenuType::QuestMenu] = new MenuQuest();
    m_ListMenu[eMenuType::StatsMenu] = new MenuStats();
    m_ListMenu[eMenuType::TitlesMenu] = new MenuTitles();
    MenuWardrobe* l_Wardrobe = new MenuWardrobe();
    m_ListMenu[eMenuType::WardrobeMenu] = l_Wardrobe;
}

MenuManager::~MenuManager()
{
}

void MenuManager::GenericAction(const uint16 & p_MenuID)
{
    switch (p_MenuID)
    {
        case 0:
            m_ListMenu[eMenuType::StatsMenu]->Open();
            break;
        case 1:
            m_ListMenu[eMenuType::TitlesMenu]->Open();
            break;
        case 2:
            m_ListMenu[eMenuType::WardrobeMenu]->Open();
            break;
        case 3:
            g_Socket->SendSave();
            Close();
            break;
        case 5:
            Close();
            break;
    }
}

void MenuManager::KeyPress(const sf::Keyboard::Key & p_Key)
{
    for (std::map<eMenuType, Menu*>::iterator l_It = m_ListMenu.begin(); l_It != m_ListMenu.end(); ++l_It)
    {
        if ((*l_It).second->IsOpen())
        {
            (*l_It).second->KeyPress(p_Key);
            return;
        }
    }

    switch (p_Key)
    {
        case sf::Keyboard::Escape:
        {
            if (IsOpen())
                Close();
            break;
        }
        case sf::Keyboard::Return:
        {
            std::pair<uint8, uint8> l_SelectedElem = GetSelectedElement();
            MenuElement* l_Elem = GetElement(l_SelectedElem.first, l_SelectedElem.second);
            if (l_Elem == nullptr)
                break;
            l_Elem->LaunchFunc(l_Elem->GetIDLabel(), this);
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
    for (std::map<eMenuType, Menu*>::iterator l_It = m_ListMenu.begin(); l_It != m_ListMenu.end(); ++l_It)
    {
        if ((*l_It).second->IsOpen())
            l_Result.push_back((*l_It).second);
    }
    return l_Result;
}

void MenuManager::SetVisualManager(VisualManager* p_Menumanager)
{
    Menu::SetVisualManager(p_Menumanager);
    m_ListMenu[eMenuType::WardrobeMenu]->SetVisualManager(p_Menumanager);
}


void MenuManager::AddElementToMenu(eMenuType p_MenuType, const uint8 & p_Col, const uint8 & p_Row, const std::string & p_Label)
{
    if (m_ListMenu.find(p_MenuType) == m_ListMenu.end())
        return;

    m_ListMenu[p_MenuType]->AddElement(p_Col, p_Row, p_Label);
}

Menu* MenuManager::GetMenu(eMenuType p_MenuType)
{
    if (m_ListMenu.find(p_MenuType) == m_ListMenu.end())
        return nullptr;

    return m_ListMenu[p_MenuType];
}
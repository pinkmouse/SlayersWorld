#include "MenuQuest.hpp"
#include "../../Global.hpp"

MenuQuest::MenuQuest() :
    Menu(5, 20)
{
    m_Pos.x = 120;
    m_Pos.y = 30;
}

MenuQuest::~MenuQuest()
{
}

void MenuQuest::KeyPress(const sf::Keyboard::Key & p_Key)
{
    switch (p_Key)
    {
        case sf::Keyboard::Escape:
        {
            if (IsOpen())
                Close();
            break;
        }
    }
}

#include "MenuQuest.hpp"
#include "../../Global.hpp"

MenuQuest::MenuQuest() :
    Menu(5, 20)
{
    m_Pos.x = 120;
    m_Pos.y = 30;
    AddElement(0, 0, "Save");
}

MenuQuest::~MenuQuest()
{
}

void MenuQuest::KeyPress(const eKeyBoardAction & p_Key)
{
}

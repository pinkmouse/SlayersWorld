#include "MenuMain.hpp"
#include "../../Global.hpp"

MenuMain::MenuMain() :
    Menu(1, 6)
{
    m_Pos.x = 30;
    m_Pos.y = 30;
    AddElement(0, 0, "Save");
}

MenuMain::~MenuMain()
{
}

void MenuMain::KeyPress(const eKeyBoardAction & p_Key)
{
}

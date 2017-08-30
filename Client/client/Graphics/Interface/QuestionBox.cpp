#include "QuestionBox.hpp"
#include "../../Global.hpp"

QuestionBox::QuestionBox()
{
}

QuestionBox::QuestionBox(const uint16 & p_QuestionID, const std::string & p_Title) :
    Menu(5, 1),
    m_QuestionID(p_QuestionID)
{
    m_Pos.x = 65;
    m_Pos.y = 70;

    SetTitle(p_Title);
    AddElement(1, 0, "Accepter");
    GetElement(1, 0)->SetFunc(&Menu::GenericAction, 0);
    AddElement(3, 0, "Refuser");
    GetElement(3, 0)->SetFunc(&Menu::GenericAction, 1);
    SetSelectedElement(1, 0);
}

QuestionBox::~QuestionBox()
{
}

void QuestionBox::Open()
{
    Menu::Open();
    SetSelectedElement(1, 0);
}

void QuestionBox::GenericAction(const uint16 & p_MenuID)
{
    g_Socket->SendAnswerQuestion(m_QuestionID, p_MenuID);
    Close();
}

void QuestionBox::KeyPress(const sf::Keyboard::Key & p_Key)
{
    switch (p_Key)
    {
        case sf::Keyboard::Escape:
        {
            if (IsOpen())
                Close();
            break;
        }
        case sf::Keyboard::Space:
        {
            std::pair<uint8, uint8> l_SelectedElem = GetSelectedElement();
            MenuElement* l_Elem = GetElement(l_SelectedElem.first, l_SelectedElem.second);
            if (l_Elem == nullptr)
                break;
            l_Elem->LaunchFunc(l_Elem->GetIDLabel(), this);
            break;
        }
        case sf::Keyboard::Up:
        {
            SelectNextElementOn(Orientation::Up);
            break;
        }
        case sf::Keyboard::Down:
        {
            SelectNextElementOn(Orientation::Down);
            break;
        }
        case sf::Keyboard::Left:
        {
            SelectNextElementOn(Orientation::Left);
            break;
        }
        case sf::Keyboard::Right:
        {
            SelectNextElementOn(Orientation::Right);
            break;
        }
    }
}

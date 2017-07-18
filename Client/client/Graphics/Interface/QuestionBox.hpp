#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class QuestionBox : public Menu
{
public:
    QuestionBox();
    QuestionBox(const uint16 &, const std::string &);
    ~QuestionBox();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void Open();

private:
    uint16 m_QuestionID;
};
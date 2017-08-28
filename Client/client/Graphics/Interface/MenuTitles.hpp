#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class MenuTitles : public Menu
{
public:
    MenuTitles();
    ~MenuTitles();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void GenericAction2(const uint16 &);
    void AddTitle(const uint16 &, const std::string &);
    void RemoveTitle(const uint16 &);

private:
    std::map<uint16, std::string> m_Titles;
};
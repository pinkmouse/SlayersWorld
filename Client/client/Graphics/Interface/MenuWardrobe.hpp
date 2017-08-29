#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class MenuWardrobe : public Menu
{
public:
    MenuWardrobe();
    ~MenuWardrobe();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void GenericAction2(const uint16 &);
    void AddSkin(const uint16 &, const std::string &);
    void RemoveSkin(const uint16 &);

private:
    std::map<uint16, std::string> m_Skins;
};
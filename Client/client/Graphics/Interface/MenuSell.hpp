#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class SubMenuSell : public Menu
{
public:
    SubMenuSell();
    ~SubMenuSell();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void SetCurrentItem(const Item &, const uint8 &);
    void SetCurrentItemPrice(const uint16 &);

private:
    std::pair<Item, uint8> m_CurrentItem;
    uint16 m_CurrentItemPrice;
};

class MenuSell : public Menu
{
public:
    MenuSell();
    ~MenuSell();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void AddItem(const uint8 &, const Item &);
    void AddItemPrice(const uint8 &, const uint16 &);
    Item* GetItem(const uint8 &);
    void SetStackItem(const uint8 &, const uint8 & );
    void RemoveItem(const uint8 &);
    void SetSize(const uint8 &);
    void SetCurrency(const eTypeCurrency &, const uint16 &);

private:
    std::map<uint8, Item>           m_Items;
    std::map<uint8, uint16>         m_ItemsPrice;
    std::map<eTypeCurrency, uint16> m_Currencies;
    uint8                           m_Size;
    SubMenuSell                     m_SubMenu;
};
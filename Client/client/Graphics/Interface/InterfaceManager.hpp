#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "../TileSprite.hpp"
#include "../../Define.hpp"
#include "../Window.hpp"
#include "../../World/Events/Events.hpp"
#include "WritingField.hpp"

class InterfaceManager
{
public:
    InterfaceManager(Events*);
    ~InterfaceManager();

    void Initialize();
    TileSprite GetField(uint16, uint16);
    void Draw(Window &);

private:
    sf::Texture     m_SystemTexture;
    Events*         m_Events;
    WritingField*   m_WritingField;
};


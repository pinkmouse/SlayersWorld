#pragma once
#include "../Define.hpp"
#include <SFML/Graphics/Texture.hpp>
#include "SkinSprite.hpp"
#include "Skin.hpp"
#include <map>

class SkinsManager
{
public:
    SkinsManager();
    ~SkinsManager();
    bool LoadSkins();
    SkinSprite* GetSkinSprite(uint8, uint8);

private:
    std::map<uint8, Skin*> m_SkinsMap;
    std::map<uint8, sf::Texture*> m_TextureSkinsMap;
};
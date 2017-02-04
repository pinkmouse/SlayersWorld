#pragma once
#include "../Define.hpp"
#include <SFML/Graphics/Texture.hpp>
#include "SkinSprite.hpp"
#include "Visual.hpp"
#include "VisualEffect.hpp"
#include <map>

class VisualManager
{
public:
    VisualManager();
    ~VisualManager();
    bool LoadSkins();
    bool LoadVisuals();
    SkinSprite* GetVisualSprite(eVisualType, uint8, uint8);
    Visual* GetVisual(eVisualType, uint8);

private:
    std::map< eVisualType , std::map<uint8, Visual> >m_VisualsMap;
    std::map<uint8, sf::Texture*> m_TextureSkinsMap;
};
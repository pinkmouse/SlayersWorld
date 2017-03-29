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
    SkinSprite* GetVisualSprite(eVisualType, int16, uint8);
    Visual* GetVisual(eVisualType, int16);

private:
    std::map< eVisualType , std::map<int16, Visual> >m_VisualsMap;
    std::map<int16, sf::Texture*> m_TextureSkinsMap;
};
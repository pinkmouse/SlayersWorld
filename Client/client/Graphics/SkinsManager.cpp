#include "SkinsManager.hpp"


SkinsManager::SkinsManager()
{
}


SkinsManager::~SkinsManager()
{
}

SkinSprite* SkinsManager::GetSkinSprite(uint8 p_SkinID, uint8 p_Position)
{
    return m_SkinsMap[p_SkinID]->at(p_Position);
}

bool SkinsManager::LoadSkins()
{
    std::string l_FileName = "skin.png";
    sf::Texture *l_Texture = new sf::Texture();
    if (!l_Texture->loadFromFile(SKINS_FOLDER + l_FileName))
    {
        printf("Load Skin %s Failed\n", l_FileName.c_str());
        return false;
    }
    m_TextureSkinsMap[0] = l_Texture;
    std::vector<SkinSprite*>* l_SkinSpriteList = new std::vector<SkinSprite*>();
    for (int i = 0; i < 12; ++i)
    {
        SkinSprite* l_SkinSprite = new SkinSprite();
        l_SkinSprite->setTexture(*l_Texture);
        l_SkinSprite->setTextureRect(sf::IntRect((i % 3) * SKIN_SIZE_X, (i / 3) * SKIN_SIZE_Y, SKIN_SIZE_X, SKIN_SIZE_Y));
        l_SkinSpriteList->push_back(l_SkinSprite);
    }

    m_SkinsMap[0] = l_SkinSpriteList;
    return true;
}
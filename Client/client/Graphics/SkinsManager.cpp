#include "SkinsManager.hpp"


SkinsManager::SkinsManager()
{
}


SkinsManager::~SkinsManager()
{
}

SkinSprite* SkinsManager::GetSkinSprite(uint8 p_SkinID, uint8 p_Position)
{
    return m_SkinsMap[p_SkinID]->GetSprite(p_Position);
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

    uint32 l_NbSkinX = l_Texture->getSize().x / SKIN_SIZE_X;
    uint32 l_NbSkinY = l_Texture->getSize().y / SKIN_SIZE_Y;
    Skin* l_Skin = new Skin(l_NbSkinX, l_NbSkinY);

    for (int i = 0; i < l_NbSkinX * l_NbSkinY; ++i)
    {
        SkinSprite* l_SkinSprite = new SkinSprite(l_NbSkinX, l_NbSkinY);
        l_SkinSprite->setTexture(*l_Texture);
        l_SkinSprite->setTextureRect(sf::IntRect((i % l_NbSkinX) * SKIN_SIZE_X, (i / l_NbSkinX) * SKIN_SIZE_Y, SKIN_SIZE_X, SKIN_SIZE_Y));
        l_Skin->AddSprite(l_SkinSprite);
    }

    m_SkinsMap[0] = l_Skin;
    return true;
}
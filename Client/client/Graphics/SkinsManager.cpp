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

Skin* SkinsManager::GetSkin(uint8 p_SkinID)
{
    return m_SkinsMap[p_SkinID];
}

bool SkinsManager::LoadSkins()
{
    sf::Texture *l_Texture = nullptr;
    for (uint8 i = 0; i < 68; ++i)
    {
        l_Texture = new sf::Texture();
        std::string l_FileName = std::to_string(i) + ".png";
        if (!l_Texture->loadFromFile(SKINS_FOLDER + l_FileName))
        {
            printf("Load Skin %s Failed\n", l_FileName.c_str());
            return false;
        }
        m_TextureSkinsMap[i] = l_Texture;

        uint32 l_NbSkinX = l_Texture->getSize().x / SKIN_SIZE_X;
        uint32 l_NbSkinY = l_Texture->getSize().y / SKIN_SIZE_Y;
        Skin* l_Skin = new Skin(l_NbSkinX, l_NbSkinY);

        for (uint8 j = 0; j < l_NbSkinX / MAX_MOVEMENT_POSITION; ++j)
        {
            for (uint8 i = 0; i < MAX_MOVEMENT_POSITION * Orientation::MAX; ++i)
            {
                SkinSprite* l_SkinSprite = new SkinSprite(l_NbSkinX, l_NbSkinY);
                l_SkinSprite->setTexture(*l_Texture);
                l_SkinSprite->setTextureRect(sf::IntRect((i % MAX_MOVEMENT_POSITION) * SKIN_SIZE_X + (j * MAX_MOVEMENT_POSITION * SKIN_SIZE_X), (i / MAX_MOVEMENT_POSITION) * SKIN_SIZE_Y, SKIN_SIZE_X, SKIN_SIZE_Y));
                l_Skin->AddSprite(l_SkinSprite);
            }
        }

        m_SkinsMap[i] = l_Skin;
    }
    return true;
}
#include "VisualManager.hpp"

VisualManager::VisualManager()
{
}

VisualManager::~VisualManager()
{
}

SkinSprite* VisualManager::GetVisualSprite(eVisualType p_Type, int16 p_VisualID, uint8 p_Position)
{
    if (m_VisualsMap.find(p_Type) == m_VisualsMap.end())
    {
        return nullptr;
    }

    if (m_VisualsMap[p_Type].find(p_VisualID) == m_VisualsMap[p_Type].end())
        return nullptr;

    return m_VisualsMap[p_Type][p_VisualID].GetSprite(p_Position);
}

Visual* VisualManager::GetVisual(eVisualType p_Type, int16 p_VisualID)
{
    if (m_VisualsMap.find(p_Type) == m_VisualsMap.end())
        return nullptr;

    if (m_VisualsMap[p_Type].find(p_VisualID) == m_VisualsMap[p_Type].end())
        return nullptr;

    return &m_VisualsMap[p_Type][p_VisualID];
}

bool VisualManager::LoadSkins()
{
    sf::Texture *l_Texture = nullptr;

    /// SKINS
    for (uint8 i = 0; i < MAX_VISUAL_CHECK/*MAX_SKIN_IMG*/; ++i)
    {
        l_Texture = new sf::Texture();
        std::string l_FileName = std::to_string(i) + ".png";
        if (!l_Texture->loadFromFile(SKINS_FOLDER + l_FileName))
        {
            break;
            //printf("Load Skin %s Failed\n", l_FileName.c_str());
            //return false;
        }
        m_TextureSkinsMap[i] = l_Texture;

        uint32 l_NbSkinX = l_Texture->getSize().x / SKIN_SIZE_X;
        uint32 l_NbSkinY = l_Texture->getSize().y / SKIN_SIZE_Y;
        Visual l_Skin(eVisualType::VisualSkin, l_NbSkinX, l_NbSkinY);

        for (uint8 j = 0; j < l_NbSkinX / MAX_MOVEMENT_POSITION; ++j)
        {
            for (uint8 i = 0; i < MAX_MOVEMENT_POSITION * Orientation::MAX; ++i)
            {
                SkinSprite l_SkinSprite(l_NbSkinX, l_NbSkinY);
                l_SkinSprite.setTexture(*l_Texture);
                l_SkinSprite.setTextureRect(sf::IntRect((i % MAX_MOVEMENT_POSITION) * SKIN_SIZE_X + (j * MAX_MOVEMENT_POSITION * SKIN_SIZE_X), (i / MAX_MOVEMENT_POSITION) * SKIN_SIZE_Y, SKIN_SIZE_X, SKIN_SIZE_Y));
                l_Skin.AddSprite(l_SkinSprite);
            }
        }

        m_VisualsMap[l_Skin.GetType()][i] = l_Skin;
    }

    /// SKINS MOUNT
    for (uint8 i = 0; i < MAX_VISUAL_CHECK/*MAX_SKIN_MOUNT_IMG*/; ++i)
    {
        l_Texture = new sf::Texture();
        std::string l_FileName = std::to_string(i) + ".png";
        if (!l_Texture->loadFromFile(SKINS_MOUNT_FOLDER + l_FileName))
        {
            break;
            //printf("Load Skin %s Failed\n", l_FileName.c_str());
            //return false;
        }
        m_TextureSkinsMap[i] = l_Texture;

        uint32 l_NbSkinX = l_Texture->getSize().x / SKIN_MOUNT_SIZE_X;
        uint32 l_NbSkinY = l_Texture->getSize().y / SKIN_MOUNT_SIZE_Y;
        Visual l_Skin(eVisualType::VisualSkinMount, l_NbSkinX, l_NbSkinY);

        for (uint8 j = 0; j < l_NbSkinX / MAX_MOVEMENT_POSITION; ++j)
        {
            for (uint8 i = 0; i < MAX_MOVEMENT_POSITION * Orientation::MAX; ++i)
            {
                SkinSprite l_SkinSprite(l_NbSkinX, l_NbSkinY);
                l_SkinSprite.setTexture(*l_Texture);
                l_SkinSprite.setTextureRect(sf::IntRect((i % MAX_MOVEMENT_POSITION) * SKIN_MOUNT_SIZE_X + (j * MAX_MOVEMENT_POSITION * SKIN_MOUNT_SIZE_X), (i / MAX_MOVEMENT_POSITION) * SKIN_MOUNT_SIZE_Y, SKIN_MOUNT_SIZE_X, SKIN_MOUNT_SIZE_Y));
                l_Skin.AddSprite(l_SkinSprite);
            }
        }
        m_VisualsMap[l_Skin.GetType()][i] = l_Skin;
    }

    /// SKINS ANIMATIONUNIT
    for (uint8 i = 0; i < MAX_VISUAL_CHECK/*MAX_SKIN_ANMATIONUNIT_IMG*/; ++i)
    {
        l_Texture = new sf::Texture();
        std::string l_FileName = std::to_string(i) + ".png";
        if (!l_Texture->loadFromFile(SKINS_ANIMATIONUNIT_FOLDER + l_FileName))
        {
            break;
            //printf("Load Skin %s Failed\n", l_FileName.c_str());
            //return false;
        }
        m_TextureSkinsMap[i] = l_Texture;

        uint32 l_NbSkinX = l_Texture->getSize().x / SKIN_SIZE_X;
        uint32 l_NbSkinY = l_Texture->getSize().y / SKIN_SIZE_Y;
        Visual l_Skin(eVisualType::VisualAnimationUnit, l_NbSkinX, l_NbSkinY);

        for (uint8 j = 0; j < l_NbSkinY; ++j)
        {
            for (uint8 k = 0; k < l_NbSkinX; ++k)
            {
                SkinSprite l_SkinSprite(l_NbSkinX, l_NbSkinY);
                l_SkinSprite.setTexture(*l_Texture);
                l_SkinSprite.setTextureRect(sf::IntRect(k * SKIN_SIZE_X, j * SKIN_SIZE_Y, SKIN_SIZE_X, SKIN_SIZE_Y));
                l_Skin.AddSprite(l_SkinSprite);
            }
        }

        m_VisualsMap[l_Skin.GetType()][i] = l_Skin;
    }

    return true;
}

bool VisualManager::LoadVisuals()
{
    sf::Texture *l_Texture = nullptr;

    /// SPELL IMG
    for (uint8 i = 0; i < MAX_VISUAL_CHECK/*MAX_SPELL_IMG*/; ++i)
    {
        l_Texture = new sf::Texture();
        std::string l_FileName = std::to_string(i) + ".png";
        if (!l_Texture->loadFromFile(SPELLS_FOLDER + l_FileName))
        {
            break;
            //printf("Load Spell %s Failed\n", l_FileName.c_str());
            //return false;
        }
        m_TextureSkinsMap[i] = l_Texture;

        uint32 l_VisuSizeX = l_Texture->getSize().x / MAX_VISUAL_IMG_X;
        uint32 l_VisuSizeY = l_Texture->getSize().y / MAX_VISUAL_IMG_Y;
        Visual l_Visual(eVisualType::VisualSpell, MAX_VISUAL_IMG_X, 0);

        for (uint8 j = 0; j < MAX_VISUAL_IMG_Y; ++j)
        {
            for (uint8 k = 0; k < MAX_VISUAL_IMG_X; ++k)
            {
                SkinSprite l_SkinSprite(l_VisuSizeX, l_VisuSizeY);
                l_SkinSprite.setTexture(*l_Texture);
                l_SkinSprite.setTextureRect(sf::IntRect(k * l_VisuSizeX, j * l_VisuSizeY, l_VisuSizeX, l_VisuSizeY));
                l_Visual.AddSprite(l_SkinSprite);
            }
        }

        m_VisualsMap[l_Visual.GetType()][i] = l_Visual;
    }

    for (uint8 i = 0; i < MAX_VISUAL_CHECK/*MAX_GOB_IMG*/; ++i)
    {
        l_Texture = new sf::Texture();
        std::string l_FileName = std::to_string(i) + ".png";
        if (!l_Texture->loadFromFile(GOBS_SKIN_FOLDER + l_FileName))
        {
            break;
            //printf("Load Gob %s Failed\n", l_FileName.c_str());
            //return false;
        }
        m_TextureSkinsMap[i] = l_Texture;

        uint32 l_NbVisuSizeX = l_Texture->getSize().x / MAX_GOB_IMG_X;
        uint32 l_NbVisuSizeY = l_Texture->getSize().y;
        Visual l_Visual(eVisualType::VisualGob, MAX_GOB_IMG_X, 0);

        for (uint8 j = 0; j < MAX_GOB_IMG_X; ++j)
        {
            SkinSprite l_SkinSprite(l_NbVisuSizeX, l_NbVisuSizeY);
            l_SkinSprite.setTexture(*l_Texture);
            l_SkinSprite.setTextureRect(sf::IntRect(j * l_NbVisuSizeX, 0, l_NbVisuSizeX, l_NbVisuSizeY));
            l_Visual.AddSprite(l_SkinSprite);
        }

        m_VisualsMap[l_Visual.GetType()][i] = l_Visual;
    }


    for (uint8 i = 0; i < MAX_VISUAL_CHECK/*MAX_LABELL_IMG;*/; ++i)
    {
        l_Texture = new sf::Texture();
        std::string l_FileName = std::to_string(i) + ".png";
        if (!l_Texture->loadFromFile(LABEL_FOLDER + l_FileName))
        {
            break;
            //printf("Load Label %s Failed\n", l_FileName.c_str());
            //return false;
        }

        Visual l_Visual(eVisualType::VisualLabel, MAX_GOB_IMG_X, 0);

        for (uint8 j = 0; j < l_Texture->getSize().y / LABEL_IMG_Y; ++j)
        {
            SkinSprite l_SkinSprite(LABEL_IMG_X, LABEL_IMG_Y);
            l_SkinSprite.setTexture(*l_Texture);
            l_SkinSprite.setTextureRect(sf::IntRect(j, j * LABEL_IMG_Y, LABEL_IMG_X, LABEL_IMG_Y));
            l_Visual.AddSprite(l_SkinSprite);
        }

        m_VisualsMap[l_Visual.GetType()][i] = l_Visual;
    }
    return true;
}
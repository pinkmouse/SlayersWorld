#include "TileSet.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>



TileSet::TileSet()
{
	std::string l_FileName = "tileset.png";
	if (!m_Texture.loadFromFile(TILE_FOLDER + l_FileName, sf::IntRect(0, 0, 480, 8000)))
		printf("Load TileSet Failed\n");
	else
	{
		m_X = m_Texture.getSize().x;
		m_Y = m_Texture.getSize().y;
		m_XCase = m_X / TILE_SIZE;
		m_YCase = m_Y / TILE_SIZE;
	}
}


TileSet::~TileSet()
{
}

void TileSet::BuildSprites()
{
	int32 l_TileSizeNumber = m_XCase * m_YCase;

	for (int32 i = 0; i < l_TileSizeNumber; ++i)
		m_AllTileSprite.push_back(GetNewTileSprite(i));

}

TileSprite* TileSet::GetNewTileSprite(uint16 p_ID)
{
	TileSprite* l_TileSprite = new TileSprite();
	l_TileSprite->setTexture(m_Texture);
	l_TileSprite->setTextureRect(sf::IntRect((p_ID % m_XCase) * TILE_SIZE, (p_ID / m_XCase) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	return l_TileSprite;
}

size_t TileSet::GetTileListSize() const
{
	return m_AllTileSprite.size();
}

TileSprite* TileSet::GetTileSprite(uint16 p_ID)
{
	return m_AllTileSprite[p_ID];
}

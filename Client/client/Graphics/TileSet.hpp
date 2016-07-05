#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "../Define.hpp"
#include "TileSprite.hpp"
#include <vector>

class TileSet
{
public:
	TileSet();
	~TileSet();
	void FillSpriteList();
	TileSprite* GetTileSprite(uint16);
	TileSprite* GetNewTileSprite(uint16);
	size_t GetTileListSize() const;

private:
	std::vector<TileSprite*> m_AllTileSprite;
	sf::Texture m_Texture;
	uint32	m_X;
	uint32	m_Y;
	uint16	m_XCase;
	uint16	m_YCase;
};


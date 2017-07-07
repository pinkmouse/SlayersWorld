#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "../Define.hpp"
#include "TileSprite.hpp"
#include <vector>

class TileSet
{
public:
	TileSet(const std::string &);
	~TileSet();
	void BuildSprites();
	TileSprite* GetTileSprite(int16) const;
	TileSprite* GetNewTileSprite(uint16);
	size_t GetTileListSize() const;
    std::string GetFileName() const;

private:
	std::vector<TileSprite*> m_AllTileSprite;
	sf::Texture m_Texture;
    std::string m_FileName;
	uint32	m_X;
	uint32	m_Y;
	uint16	m_XCase;
	uint16	m_YCase;
};


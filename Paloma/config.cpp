#include "config.h"

Config::Config() :
    m_TileLevel(1)
{

}

void Config::SetTileLevel(int p_TileLevel)
{
    m_TileLevel = p_TileLevel;
}

int Config::GetTileLevel() const
{
    return m_TileLevel;
}

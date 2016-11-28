#pragma once

#include "Entities/Player.hpp"
#include "Socket/Socket.hpp"
#include <SFML/Graphics/Font.hpp>
#include "World/ConfigHandler.hpp"

extern ConfigHandler*   g_Config;
extern Player*          g_Player;
extern Socket*          g_Socket;
extern sf::Font*        g_Font;
#pragma once

#include <SFML/Network/Packet.hpp>
#include "../Define.hpp"

class WorldPacket : public sf::Packet
{
public:
	WorldPacket();
	~WorldPacket();
};


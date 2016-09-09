#pragma once
#include <SFML/Network/Packet.hpp>

class WorldPacket : public sf::Packet
{
public:
	WorldPacket();
	~WorldPacket();
};

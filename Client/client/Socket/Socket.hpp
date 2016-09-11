#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "../Define.hpp"

class Socket : public sf::TcpSocket
{
public:
	Socket();
	~Socket();
	bool Connection();
	void SendAuth(const std::string &, const std::string &);
    void SendGoDirection(const uint8&, const uint32&, const uint32&);
    void SendStopMovement(const uint32&, const uint32&);
    void SendUnitUnknow(const uint8&, const uint16&);
    void SendTalk(const std::string &);
};

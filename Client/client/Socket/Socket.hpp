#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "../World/WorldPacket.hpp"
#include "../Define.hpp"

class Socket : public sf::TcpSocket
{
public:
	Socket();
	~Socket();
	bool Connection(const std::string &);
    sf::Socket::Status Receive(WorldPacket&);
	void SendAuth(const std::string &, const std::string &);
    void SendGoDirection(const uint8&, const uint32&, const uint32&);
    void SendStopMovement(const uint32&, const uint32&);
    void SendStartAttack();
    void SendStopAttack();
    void SendLoadingPong();
    void SendEventAction(const uint8&);
    void SendUnitUnknow(const uint8&, const uint16&);
    void SendTalk(const std::string &);
    void SendSave();
    void SendStatAction(eStats, bool, uint8);
    void SendAnswerQuestion(const uint16 &, const uint8 &_Resp);
    bool IsConnected() const;

private:
    bool m_Connected;
};

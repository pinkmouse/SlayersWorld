#include "Socket.hpp"

#include "../Define.hpp"
#include <SFML/Network/IpAddress.hpp>
#include "../World/WorldPacket.hpp"
#include "../World/PacketDefine.hpp"


Socket::Socket() :
    m_Connected(false)
{
}


Socket::~Socket()
{
}

bool Socket::Connection(const std::string & p_Ip)
{
    //printf("Ip = %s\n", p_Ip.c_str());
	sf::Socket::Status l_Status = this->connect(sf::IpAddress(p_Ip), PORT_SERVER);

	if (l_Status == sf::Socket::Status::Done)
    {
        m_Connected = true;
		return true;
    }
    else
        m_Connected = false;
	return false;
}

void Socket::SendAuth(const std::string & l_Login, const std::string & l_Password)
{
	WorldPacket packet;
	uint8 l_ID = CMSG::C_Connexion;

	packet << l_ID << l_Login << l_Password;

	send(packet);
}

void Socket::SendGoDirection(const uint8& p_Orientation, const uint32& p_PosX, const uint32& p_PosY)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitGoDirection;

    packet << l_ID << p_Orientation << (uint16)p_PosX << (uint16)p_PosY;

    send(packet);
}

void Socket::SendStopMovement(const uint32& p_Posx, const uint32& p_PosY)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitStopMovement;

    packet << l_ID << (uint16)p_Posx << (uint16)p_PosY;

    send(packet);
}

void Socket::SendStartAttack()
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitStartAttack;

    packet << l_ID;

    send(packet);
}

void Socket::SendStopAttack()
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitStopAttack;

    packet << l_ID;

    send(packet);
}

void Socket::SendUnitUnknow(const uint8& p_TypeID, const uint16& p_ID)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitCreate;

    packet << l_ID << p_TypeID << p_ID;

    send(packet);
}

void Socket::SendTalk(const std::string & p_TalkString)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitTalk;

    packet << l_ID << p_TalkString;

    send(packet);
}

void Socket::SendEventAction(const uint8 & p_Key)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitEventAction;

    packet << l_ID << p_Key;

    send(packet);
}

bool Socket::IsConnected() const
{
    return m_Connected;
}

sf::Socket::Status Socket::Receive(WorldPacket & p_Packet)
{
    sf::Socket::Status l_Status = sf::TcpSocket::receive(p_Packet);
    if (l_Status == sf::Socket::Status::Disconnected)
        m_Connected = false;
    return l_Status;
}

void Socket::SendLoadingPong()
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_LoadingPong;

    packet << l_ID;

    send(packet);
}

void Socket::SendSave()
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_Save;

    packet << l_ID;

    send(packet);
}

void Socket::SendStatAction(eStats p_Stat, bool p_Add, uint8 p_Nb)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_StatAction;
    uint8 l_TypeStat = (uint8)p_Stat;

    packet << l_ID;
    packet << l_TypeStat;
    packet << p_Add;
    packet << p_Nb;

    send(packet);
}

void Socket::SendAnswerQuestion(const uint16 & p_QuestionID, const uint8 & p_Resp)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UnitAnswerQuestion;

    packet << l_ID;
    packet << p_QuestionID;
    packet << p_Resp;

    send(packet);
}

void Socket::SendUpdateTitle(bool p_Apply, const uint16 & p_TitleID)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UpdateTitle;

    packet << l_ID;
    packet << p_Apply;
    packet << p_TitleID;

    send(packet);
}

void Socket::SendUpdateSkin(const uint16 & p_SkinID)
{
    WorldPacket packet;
    uint8 l_ID = CMSG::C_UpdateSkin;

    packet << l_ID;
    packet << p_SkinID;

    send(packet);
}
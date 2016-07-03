#include "PacketHandler.hpp"

PacketHandler::PacketHandler()
{
}


PacketHandler::~PacketHandler()
{
}

void PacketHandler::LoadPacketHandlerMap()
{
    m_PacketHandleMap[1] = &PacketHandler::HandleConnexion;
}

void PacketHandler::HandleConnexion(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
	std::string l_Login;
	std::string l_Password;
	p_Packet >> l_Login;
	p_Packet >> l_Password;

	if (g_Config->IsPositiveValue("LoginDebug"))
	{
		p_WorldSocket->SendAuthResponse(0);
		return;
	}
	/// TO-DO
	/// Check Auth
}

void PacketHandler::OperatePacket(WorldPacket &p_Packet, WorldSocket* p_WorldSocket)
{
    uint8 l_PacketID;
    p_Packet >> l_PacketID;
    printf("Receive Packet %d\n", l_PacketID);
    m_Func l_Fun = m_PacketHandleMap[l_PacketID];
	if (l_Fun != nullptr)
		(this->*(l_Fun))(p_Packet, p_WorldSocket);
	else
		printf("Packet %d Unknow\n", l_PacketID);

}


#include <map>
#include "../Define.hpp"
#include "WorldPacket.hpp"
#include "../Socket/Socket.hpp"

class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();
	void LoadPacketHandlerMap();

	void OperatePacket(WorldPacket &, Socket*);
	void HandleConnexion(WorldPacket &, Socket*);

private:
	using m_Func = void(PacketHandler::*)(WorldPacket &, Socket*);

	std::map<uint8, m_Func> m_PacketHandleMap;
};


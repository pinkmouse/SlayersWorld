#include <map>
#include "../Define.hpp"
#include "WorldPacket.hpp"
#include "../Map/MapManager.hpp"
#include "../Socket/Socket.hpp"

class PacketHandler
{
public:
	PacketHandler(MapManager*);
	~PacketHandler();
	void LoadPacketHandlerMap();

	void OperatePacket(WorldPacket &);
	void HandleConnexion(WorldPacket &);
    void HandleCreatePlayer(WorldPacket &);
    void HandleStopMovement(WorldPacket &);

private:
	MapManager* m_MapManager;
	using m_Func = void(PacketHandler::*)(WorldPacket &);

	std::map<uint8, m_Func> m_PacketHandleMap;
};


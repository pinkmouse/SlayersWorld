#include <map>
#include "../Define.hpp"
#include "WorldPacket.hpp"
#include "../Map/MapManager.hpp"
#include "../Socket/Socket.hpp"
#include "../Graphics/Interface/InterfaceManager.hpp"

class PacketHandler
{
public:
	PacketHandler(MapManager*, InterfaceManager*);
	~PacketHandler();
	void LoadPacketHandlerMap();
    bool HasMinimalRequiered() const;

	void OperatePacket(WorldPacket &);
	void HandleConnexion(WorldPacket &);
    void HandleCreateMainPlayer(WorldPacket &);
    void HandleCreateUnit(WorldPacket &);
    void HandleStopMovement(WorldPacket &);
    void HandleRemoveUnit(WorldPacket &);
    void HandleUpdateHealth(WorldPacket &);
    void HandleUpdateXpPct(WorldPacket &);
    void HandleUpdatePosition(WorldPacket &);
    void HandleUpdateOrientation(WorldPacket &);
    void HandleUnitGoDirection(WorldPacket &);
    void HandleUnitStartAttack(WorldPacket &);
    void HandleUnitStopAttack(WorldPacket &);
    void HandleTalk(WorldPacket &);
    void HandleSrvPlayerMsg(WorldPacket &);
    void HandleUpdateSkin(WorldPacket &);

private:
	MapManager* m_MapManager;
    InterfaceManager* m_InterfaceManager;
	using m_Func = void(PacketHandler::*)(WorldPacket &);

	std::map<uint8, m_Func> m_PacketHandleMap;
    bool p_HasMinimalRequiered;
};


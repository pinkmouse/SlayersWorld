#include <map>
#include "../Define.hpp"
#include "../Map/MapManager.hpp"
#include "../Socket/Socket.hpp"
#include <Nazara/Network/NetPacket.hpp>

class PacketHandler
{
public:
	PacketHandler(MapManager*);
	~PacketHandler();
	void LoadPacketHandlerMap();
    bool HasMinimalRequiered() const;

	void OperatePacket(Nz::NetPacket&);
	void HandleConnexion(Nz::NetPacket&);
    void HandleCreateMainPlayer(Nz::NetPacket&);
    void HandleCreateUnit(Nz::NetPacket&);
    void HandleStopMovement(Nz::NetPacket&);
    void HandleRemoveUnit(Nz::NetPacket&);
    void HandleUpdateHealth(Nz::NetPacket&);
    void HandleUpdatePosition(Nz::NetPacket&);
    void HandleUnitGoDirection(Nz::NetPacket&);
    void HandleUnitStartAttack(Nz::NetPacket&);
    void HandleUnitStopAttack(Nz::NetPacket&);
    void HandleTalk(Nz::NetPacket&);

private:
	MapManager* m_MapManager;
	using m_Func = void(PacketHandler::*)(Nz::NetPacket&);

	std::map<uint8, m_Func> m_PacketHandleMap;
    bool p_HasMinimalRequiered;
};


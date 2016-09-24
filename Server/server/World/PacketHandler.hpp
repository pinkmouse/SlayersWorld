#pragma once

#include <map>
#include "../Define.hpp"
#include "WorldSocket.hpp"
#include "SqlManager.hpp"
#include "../Global.hpp"
#include "../Map/MapManager.hpp"
#include <Nazara/Network/NetPacket.hpp>

class PacketHandler
{
public:
    PacketHandler(SqlManager*, MapManager*);
    ~PacketHandler();
    void LoadPacketHandlerMap();

    void OperatePacket(Nz::NetPacket&, WorldSocket*);
    void HandleConnexion(Nz::NetPacket&, WorldSocket*);
    void HandleGoDirection(Nz::NetPacket&, WorldSocket*);
    void HandleUnitUnknow(Nz::NetPacket&, WorldSocket*);
    void HandleStopMovement(Nz::NetPacket&, WorldSocket*);
    void HandleTalk(Nz::NetPacket&, WorldSocket*);
    void HandleStartAttack(Nz::NetPacket&, WorldSocket*);
    void HandleStopAttack(Nz::NetPacket&, WorldSocket*);
    void HandleDisconnected(WorldSocket*);

private:
    using m_Func = void(PacketHandler::*)(Nz::NetPacket&, WorldSocket*);

    std::map<uint16, m_Func> m_PacketHandleMap;
    SqlManager* m_SqlManager;
    MapManager* m_MapManager;
};


#pragma once

#include <map>
#include "../Define.hpp"
#include "WorldPacket.hpp"
#include "WorldSocket.hpp"
#include "SqlManager.hpp"
#include "../Global.hpp"
#include "../Map/MapManager.hpp"

class PacketHandler
{
public:
    PacketHandler(SqlManager*, MapManager*);
    ~PacketHandler();
    void LoadPacketHandlerMap();

    void OperatePacket(WorldPacket &, WorldSocket*);
    void HandleConnexion(WorldPacket &, WorldSocket*);
    void HandleGoDirection(WorldPacket &, WorldSocket*);
    void HandleStopMovement(WorldPacket &, WorldSocket*);

private:
    using m_Func = void(PacketHandler::*)(WorldPacket &, WorldSocket*);

    std::map<uint8, m_Func> m_PacketHandleMap;
    SqlManager* m_SqlManager;
    MapManager* m_MapManager;
};


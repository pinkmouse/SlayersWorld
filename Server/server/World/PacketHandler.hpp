#pragma once

#include <map>
#include "../Define.hpp"
#include "WorldPacket.hpp"
#include "WorldSocket.hpp"

class PacketHandler
{
public:
    PacketHandler();
    ~PacketHandler();
    void LoadPacketHandlerMap();

    void OperatePacket(WorldPacket &, WorldSocket*);
    void HandleConnexion(WorldPacket &, WorldSocket*);

private:
    typedef void(PacketHandler::*m_Func)(WorldPacket &, WorldSocket*);
    std::map<uint8, m_Func> m_PacketHandleMap;
};


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
    PacketHandler();
    ~PacketHandler();
    void LoadPacketHandlerMap();

    void OperatePacket(WorldPacket &, WorldSocket*);
    void HandleConnexion(WorldPacket &, WorldSocket*);
    void HandleSave(WorldPacket &, WorldSocket*);
    void HandleStatAction(WorldPacket &, WorldSocket*);
    void HandleGoDirection(WorldPacket &, WorldSocket*);
    void HandleUnitUnknow(WorldPacket &, WorldSocket*);
    void HandleStopMovement(WorldPacket &, WorldSocket*);
    void HandleLoadingPong(WorldPacket &, WorldSocket*);
    void HandleTalk(WorldPacket &, WorldSocket*);
    void HandleStartAttack(WorldPacket &, WorldSocket*);
    void HandleStopAttack(WorldPacket &, WorldSocket*);
    void HandleEventAction(WorldPacket &, WorldSocket*);
    void HandleAnswerQuestion(WorldPacket &, WorldSocket*);
    void HandleDisconnected(WorldSocket*);

private:
    typedef void(PacketHandler::*m_Func)(WorldPacket &, WorldSocket*);

    std::map<uint8, m_Func> m_PacketHandleMap;
};


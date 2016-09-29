#include "Map.hpp"
#include "../World/WorldSocket.hpp"

void Map::SendToSet(WorldPacket p_Packet, Unit* l_Unit)
{
    if (!l_Unit->IsInWorld())
        return;

    std::vector<Square*> l_SquareSet = GetSquareSet(l_Unit->GetSquareID());
    for (Square* l_Square : l_SquareSet)
    {
        std::map<uint16, Unit*>* l_SquareList = l_Square->GetList(TypeUnit::PLAYER);
        if (l_SquareList == nullptr)
            continue;

        for (std::pair<uint16, Unit*> l_UnitPair : *l_SquareList)
        {
            Unit* l_Unit = l_UnitPair.second;

            if (l_Unit == nullptr)
                continue;

            if (Player* l_Player = l_Unit->ToPlayer())
            {
                printf("Send to %s\n", l_Player->GetName().c_str());
                l_Player->GetSession()->send(p_Packet);
            }
        }
    }
}
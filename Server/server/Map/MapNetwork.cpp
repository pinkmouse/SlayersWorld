#include "Map.hpp"
#include "../World/WorldSocket.hpp"

void Map::SendToSet(WorldPacket p_Packet, Unit* p_Unit)
{
    if (!p_Unit->IsInWorld())
        return;

    std::vector<Square*> l_SquareSet = GetSquareSet(p_Unit->GetSquareID());
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
                l_Player->GetSession()->send(p_Packet);
        }
    }
}

void Map::SendToMap(WorldPacket p_Packet)
{
    for (std::map<TypeUnit, std::map<uint16, Unit*> >::iterator l_It = m_ListUnitZone.begin(); l_It!= m_ListUnitZone.end(); l_It++)
    {
        for (std::map<uint16, Unit*>::iterator l_Itr = (*l_It).second.begin(); l_Itr != (*l_It).second.end(); l_Itr++)
        {
            Unit* l_Unit = (*l_Itr).second;

            if (l_Unit == nullptr)
                continue;

            if (Player* l_Player = l_Unit->ToPlayer())
                l_Player->GetSession()->send(p_Packet);
        }
    }
}

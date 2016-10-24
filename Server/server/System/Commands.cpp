#include "../Entities/Player.hpp"
#include "../World/PacketDefine.hpp"
#include "../Global.hpp"

void Player::InitializeCommands()
{
    m_CmdHandleMap["skin"] = &Player::HandleCommandSkin;
    m_CmdHandleMap["where"] = &Player::HandleCommandWhere;
    m_CmdHandleMap["level"] = &Player::HandleCommandLevel;
}

bool Player::HandleCommandSkin(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    uint8 l_SkinID = atoi(p_ListCmd[0].c_str());
    if (l_SkinID < 67)
        SetSkinID(l_SkinID);

    return true;
}

bool Player::HandleCommandWhere(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    std::string l_Name = p_ListCmd[0];

    int32 l_Id = g_SqlManager->GetPlayerID(l_Name);
    if (l_Id <= 0)
        return false;

    Player* l_Player = g_MapManager->GetPlayer(l_Id);

    SendMsg(l_Name + " -> Map:" + std::to_string(l_Player->GetMapID()) + " X:" + std::to_string(l_Player->GetPosX()) + " Y:" + std::to_string(l_Player->GetPosY()));

    return true;
}

bool Player::HandleCommandLevel(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    std::string l_Name = p_ListCmd[0];

    int32 l_Id = g_SqlManager->GetPlayerID(l_Name);
    if (l_Id <= 0)
        return false;

    Player* l_Player = g_MapManager->GetPlayer(l_Id);

    SendMsg(l_Name + " est de niveau " + std::to_string(l_Player->GetLevel()));

    return true;
}
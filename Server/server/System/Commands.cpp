#include "../Entities/Player.hpp"
#include "../World/PacketDefine.hpp"
#include "../Global.hpp"

void Player::InitializeCommands()
{
    m_CmdHandleMap["skin"].first = eAccessType::Dummy;
    m_CmdHandleMap["skin"].second = &Player::HandleCommandSkin;
    m_CmdHandleMap["where"].first = eAccessType::Dummy;
    m_CmdHandleMap["where"].second = &Player::HandleCommandWhere;
    m_CmdHandleMap["level"].first = eAccessType::Dummy;
    m_CmdHandleMap["level"].second = &Player::HandleCommandLevel;
    m_CmdHandleMap["points"].first = eAccessType::Dummy;
	m_CmdHandleMap["points"].second = &Player::HandleCommandAddPoint;
    m_CmdHandleMap["npc"].first = eAccessType::Moderator;
    m_CmdHandleMap["npc"].second = &Player::HandleCommandCreature;
}

bool Player::HandleCommandSkin(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    uint8 l_SkinID = atoi(p_ListCmd[0].c_str());
    if (l_SkinID > 67)
        l_SkinID = 0;

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

bool Player::HandleCommandCreature(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    if (p_ListCmd[0] == "add" && p_ListCmd.size() < 3)
    {
        uint8 l_CreatureEntry = atoi(p_ListCmd[1].c_str());

        uint16 l_Id = g_SqlManager->AddNewCreature(GetMapID(), l_CreatureEntry, GetPosX(), GetPosY());
        Creature* l_Creature = new Creature(l_Id, l_CreatureEntry, g_SqlManager->GetCreatureTemplate(l_CreatureEntry), GetMapID(), GetPosX(), GetPosY());
        Map* l_Map = g_MapManager->GetMap(GetMapID());
        l_Map->AddUnit(l_Creature);
    }
    return true;
}

bool Player::HandleCommandAddPoint(std::vector<std::string> p_ListCmd)
{
	if (p_ListCmd.empty())
	{
		SendMsg("Vos points-> " + std::to_string(GetPointsSet().m_FreePoints) + "|" + std::to_string(GetPointsSet().m_Force) + "|" + std::to_string(GetPointsSet().m_Stamina) + "|" + std::to_string(GetPointsSet().m_Dexterity));
		return true;
	}
	if (p_ListCmd.size() < 2)
	{
		SendMsg("/point add|sub force|stamina|dexterity");
		return true;
	}

	int8 l_IndexCmd = -1;
	if (p_ListCmd[0] == "add")
		l_IndexCmd = 0;
	else if (p_ListCmd[0] == "sub")
		l_IndexCmd = 1;

	int8 l_IndexPoint = -1;
	if (p_ListCmd[1] == "force")
		l_IndexPoint = 0;
	else if (p_ListCmd[1] == "stamina")
		l_IndexPoint = 1;
	else if (p_ListCmd[1] == "dexterity")
		l_IndexPoint = 2;

	int8 l_NbPoints = 1;
	if (p_ListCmd.size() >= 3)
		l_NbPoints = atoi(p_ListCmd[2].c_str());

	l_NbPoints = std::max((int8)1, l_NbPoints);

	if (l_IndexCmd < 0 || l_IndexPoint < 0)
	{
		SendMsg("/point add|sub force|stamina|dexterity");
		return true;
	}

	if (l_IndexCmd == 0)
	{
		if (l_NbPoints > m_PointsSet.m_FreePoints)
		{
			SendMsg("Vous n'avez pas assez de points");
			return true;
		}
		switch (l_IndexPoint)
		{
			case 0:
			{
				m_PointsSet.m_Force += l_NbPoints;
				break;
			}
			case 1:
			{
				m_PointsSet.m_Stamina += l_NbPoints;
				break;
			}
			case 2:
			{
				m_PointsSet.m_Dexterity += l_NbPoints;
				break;
			}
		}
		m_PointsSet.m_FreePoints -= l_NbPoints;
	}

	if (l_IndexCmd == 1)
	{
		switch (l_IndexPoint)
		{
			case 0:
			{
				if (l_NbPoints > m_PointsSet.m_Force)
				{
					SendMsg("Vous n'avez pas assez de points en force");
					return true;
				}
				m_PointsSet.m_Force -= l_NbPoints;
				break;
			}
			case 1:
			{
				if (l_NbPoints > m_PointsSet.m_Stamina)
				{
					SendMsg("Vous n'avez pas assez de points en stamina");
					return true;
				}
				m_PointsSet.m_Stamina -= l_NbPoints;
				break;
			}
			case 2:
			{
				if (l_NbPoints > m_PointsSet.m_Dexterity)
				{
					SendMsg("Vous n'avez pas assez de points en dexterity");
					return true;
				}
				m_PointsSet.m_Dexterity -= l_NbPoints;
				break;
			}
		}
		m_PointsSet.m_FreePoints += l_NbPoints;
	}

	return true;
}
#include "../Entities/Player.hpp"
#include "../World/PacketDefine.hpp"
#include "../World/WorldSocket.hpp"
#include "../Global.hpp"
#include  "../System/Quest/Quest.hpp"

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
    m_CmdHandleMap["bind"].first = eAccessType::Dummy;
    m_CmdHandleMap["bind"].second = &Player::HandleBind;
    m_CmdHandleMap["save"].first = eAccessType::Dummy;
    m_CmdHandleMap["save"].second = &Player::HandleSave;
    m_CmdHandleMap["msg"].first = eAccessType::Dummy;
    m_CmdHandleMap["msg"].second = &Player::HandleCommandWisp;
    m_CmdHandleMap["quests"].first = eAccessType::Dummy;
    m_CmdHandleMap["quests"].second = &Player::HandleCommandQuests;
    m_CmdHandleMap["join"].first = eAccessType::Dummy;
    m_CmdHandleMap["join"].second = &Player::HandleCommandJoin;
    m_CmdHandleMap["gr"].first = eAccessType::Dummy;
    m_CmdHandleMap["gr"].second = &Player::HandleCommandGroupWisp;
    m_CmdHandleMap["leave"].first = eAccessType::Dummy;
    m_CmdHandleMap["leave"].second = &Player::HandleCommandLeave;
    m_CmdHandleMap["npc"].first = eAccessType::Moderator;
    m_CmdHandleMap["npc"].second = &Player::HandleCommandCreature;
    m_CmdHandleMap["who"].first = eAccessType::Moderator;
    m_CmdHandleMap["who"].second = &Player::HandleCommandWho;
    m_CmdHandleMap["tele"].first = eAccessType::Moderator;
    m_CmdHandleMap["tele"].second = &Player::HandleCommandTeleport;
    m_CmdHandleMap["regen"].first = eAccessType::Moderator;
    m_CmdHandleMap["regen"].second = &Player::HandleRegen;
    m_CmdHandleMap["speed"].first = eAccessType::Moderator;
    m_CmdHandleMap["speed"].second = &Player::HandleCommandSpeed;
    m_CmdHandleMap["test"].first = eAccessType::Moderator;
    m_CmdHandleMap["test"].second = &Player::HandleTest;
}

bool Player::HandleTest(std::vector<std::string> p_ListCmd)
{
    if (!p_ListCmd.empty())
        return false;

    AddQuest(new Quest(g_QuestManager->GetQuestTemplate(1)));
    return true;
}

bool Player::HandleCommandQuests(std::vector<std::string> p_ListCmd)
{
    if (!p_ListCmd.empty())
        return false;

    std::map< uint16, Quest* >* l_QuestList = GetQuestList();
    for (std::map< uint16, Quest* >::iterator l_It = l_QuestList->begin(); l_It != l_QuestList->end(); ++l_It)
    {
        SendMsg("-> " + (*l_It).second->GetName());
        /// Save Objectf Progress
        std::map< uint8, ObjectifProgess* >* l_ObjectProgressList = (*l_It).second->GetObjectifsProgress();
        for (std::map< uint8, ObjectifProgess* >::iterator l_Itr = l_ObjectProgressList->begin(); l_Itr != l_ObjectProgressList->end(); ++l_Itr)
            SendMsg("---> " + (*l_It).second->GetQuestTemplate()->m_ObjectifList[(*l_Itr).first]->m_Entitled + " " + std::to_string((*l_Itr).second->m_Data0) + "/" + std::to_string((*l_It).second->GetQuestTemplate()->m_ObjectifList[(*l_Itr).first]->m_Data0));
    }
    return true;
}

bool Player::HandleRegen(std::vector<std::string> p_ListCmd)
{
    if (!p_ListCmd.empty())
        return false;

    RegenerateAll();
    return true;
}

bool Player::HandleBind(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    eKeyBoardAction l_ActionKeyID = (eKeyBoardAction)atoi(p_ListCmd[0].c_str());
    if (l_ActionKeyID >= eKeyBoardAction::MaxKeyBoard)
    {
        SendMsg("Bind: error bind unknow");
        return true;
    }

    if (p_ListCmd.size() == 1)
    {
        SendMsg("Bind: " + std::to_string(GetKeyBoardBind(l_ActionKeyID)));
    }
    else if (p_ListCmd.size() == 2)
    {
        uint8 l_KeyBind = atoi(p_ListCmd[0].c_str());
        if (l_KeyBind >= sf::Keyboard::KeyCount)
        {
            SendMsg("Bind: error key unknow");
            return true;
        }
        AddKeyBoardBind(l_ActionKeyID, l_KeyBind);
        GetSession()->SendKeyBoardBind(l_ActionKeyID, l_KeyBind);
    }
    return true;
}

bool Player::HandleCommandSkin(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    uint8 l_SkinID = atoi(p_ListCmd[0].c_str());
    if (l_SkinID > 69)
        l_SkinID = 0;

    SetSkinID(l_SkinID);

    return true;
}

bool Player::HandleSave(std::vector<std::string> p_ListCmd)
{
    if (!p_ListCmd.empty())
        return false;

    g_SqlManager->SavePlayer(this);

    return true;
}

bool Player::HandleCommandWho(std::vector<std::string> p_ListCmd)
{
    std::string l_ListNames;
    std::map<uint16, Unit*>* l_ListPlayer = GetMap()->GetListUnitType(TypeUnit::PLAYER);
    for (std::map<uint16, Unit*>::iterator l_It = l_ListPlayer->begin(); l_It != l_ListPlayer->end();)
    {
        l_ListNames += (*l_It).second->GetName();
        if (++l_It != l_ListPlayer->end())
            l_ListNames += ":";
        else
            break;
    }
    SendMsg("ListPlayer: " + l_ListNames);

    return true;
}


bool Player::HandleCommandWhere(std::vector<std::string> p_ListCmd)
{
    Player* l_Player = this;
    std::string l_Name = GetName();
    if (!p_ListCmd.empty())
    {
        l_Name = p_ListCmd[0];

        int32 l_Id = g_SqlManager->GetPlayerID(l_Name);
        if (l_Id <= 0)
            return false;

        Player* l_Player = g_MapManager->GetPlayer(l_Id);
    }

    SendMsg(l_Name + " -> Map:" + std::to_string(l_Player->GetMapID()) + " X:" + std::to_string(l_Player->GetPosX()) + " Y:" + std::to_string(l_Player->GetPosY()) + " caseNb:" + std::to_string((uint16)((l_Player->GetPosY() / TILE_SIZE) * l_Player->GetMap()->GetSizeX()) + (l_Player->GetPosX() / TILE_SIZE)));

    return true;
}

bool Player::HandleCommandLevel(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    std::string l_Name = p_ListCmd[0];

    int32 l_Id = g_SqlManager->GetPlayerID(l_Name);
    if (l_Id <= 0)
    {
        SendMsg(l_Name + " est introuvable");
        return true;
    }

    Player* l_Player = g_MapManager->GetPlayer(l_Id);
    SendMsg(l_Name + " est de niveau " + std::to_string(l_Player->GetLevel()));

    return true;
}

bool Player::HandleCommandGroupWisp(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    std::string l_Msg = "";
    for (uint8 i = 0; i < p_ListCmd.size(); ++i)
        l_Msg += " " + p_ListCmd[i];

    std::vector< std::string >* l_Groups = GetAllGroupsForType(eGroupType::SIMPLE);
    if (l_Groups == nullptr)
    {
        SendMsg("Vous ne faite pas partie d'un groupe");
        return true;
    }
    for (std::vector< std::string >::iterator l_It = l_Groups->begin(); l_It != l_Groups->end(); ++l_It)
    {
        std::vector< Unit* >* l_Units = g_GroupManager->GetUnitForGroup(eGroupType::SIMPLE, (*l_It));
        if (l_Units == nullptr)
            continue;
        for (std::vector< Unit* >::iterator l_Itr = l_Units->begin(); l_Itr != l_Units->end(); ++l_Itr)
        {
            Player* l_Player = (*l_Itr)->ToPlayer();
            if (l_Player == nullptr)
                continue;

            l_Player->SendMsg(GetName() + "(gr): " + l_Msg);
        }
    }

    return true;
}

bool Player::HandleCommandWisp(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    std::string l_Name = p_ListCmd[0];

    int32 l_Id = g_SqlManager->GetPlayerID(l_Name);
    if (l_Id <= 0)
    {
        SendMsg(l_Name + " n'existe pas");
        return true;
    }

    Player* l_Player = g_MapManager->GetPlayer(l_Id);
    if (l_Player == nullptr)
    {
        SendMsg(l_Name + " n'est pas connecté");
        return true;
    }
    std::string l_Msg = "";
    for (uint8 i = 1; i < p_ListCmd.size(); ++i)
        l_Msg += " " + p_ListCmd[i];

    l_Player->SendMsg(GetName() + "(msg): " + l_Msg);
    SendMsg(l_Player->GetName() + "(msg*): " + l_Msg);

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

bool Player::HandleCommandSpeed(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    uint8 l_Speed = atoi(p_ListCmd[0].c_str());
    SetSpeed((float)l_Speed / 10.0f);

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
		SendMsg("/points add|sub force|stamina|dexterity");
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

bool Player::HandleCommandTeleport(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;

    if (p_ListCmd.size() == 2)
    {
        uint32 l_X = atoi(p_ListCmd[0].c_str());
        uint32 l_Y = atoi(p_ListCmd[1].c_str());

        TeleportTo(l_X, l_Y, Orientation::Down);
        return true;
    }
    if (p_ListCmd.size() == 3)
    {
        uint16 l_Map = atoi(p_ListCmd[0].c_str());
        uint32 l_X = atoi(p_ListCmd[1].c_str());
        uint32 l_Y = atoi(p_ListCmd[2].c_str());

        TeleportTo(l_Map, l_X, l_Y, Orientation::Down);
        return true;
    }
    return false;
}

bool Player::HandleCommandJoin(std::vector<std::string> p_ListCmd)
{
    if (p_ListCmd.empty())
        return false;


    if (p_ListCmd.size() > 1)
        return false;

    LeaveAllGroups();
    std::vector< Unit* >* l_Units = g_GroupManager->GetUnitForGroup(eGroupType::SIMPLE, p_ListCmd[0]);
    if (l_Units != nullptr)
    {
        for (std::vector< Unit* >::iterator l_Itr = l_Units->begin(); l_Itr != l_Units->end(); ++l_Itr)
        {
            Player* l_Player = (*l_Itr)->ToPlayer();
            if (l_Player == nullptr)
                continue;

            l_Player->SendMsg(GetName() + " vient de rejoindre le groupe '" + p_ListCmd[0] + "'");
        }
    }

    EnterInGroup(eGroupType::SIMPLE, p_ListCmd[0]);
    SendMsg("Vous venez de rejoindre le groupe '" + p_ListCmd[0] + "'");

    return true;
}

bool Player::HandleCommandLeave(std::vector<std::string> p_ListCmd)
{
    if (!p_ListCmd.empty())
        return false;

    std::vector< std::string >* l_Groups = GetAllGroupsForType(eGroupType::SIMPLE);
    if (l_Groups == nullptr)
    {
        SendMsg("Vous ne faite pas partie d'un groupe");
        return true;
    }
    LeaveAllGroups();

    return true;
}
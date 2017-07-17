#pragma once
#include "WorldPacket.hpp"

enum CMSG : uint8
{
    C_Connexion = 1,
    C_Save = 2,
    C_UnitCreate = 10,
    C_LoadingPong = 11,
    C_StatAction = 12,
    C_UnitGoDirection = 20,
    C_UnitStopMovement = 21,
    C_UnitTalk = 23,
    C_UnitStartAttack = 24,
    C_UnitStopAttack = 25,
    C_UnitEventAction = 26,
};

enum SMSG : uint8
{
    S_Connexion = 1,
    S_PlayerCreate = 10,
    S_KeyBoardBind = 11,
    S_UnitCreate = 12,
    S_UnitRemove = 13,
    S_UnitUpdateResource = 14,
    S_PlayerUpdateXp = 15,
    S_LogDamage = 16,
    S_SwitchMap = 17,
    S_WarningMsg = 18,
    S_CastBar = 19,
    S_UnitGoDirection = 20,
    S_UnitStopMovement = 21,
    S_UnitUpdatePosition = 22,
    S_UnitUpdateOrientation = 23,
    S_UnitTalk = 24,
    S_SrvPlayerMsg = 25,
    S_UnitStartAttack = 26,
    S_UnitStopAttack = 27,
    S_UnitUpdateSkin = 28,
    S_UnitIsInGroup = 29,
    S_UnitPlayVisual = 30,
    S_UnitUpdateStat = 31,
    S_LoadingPing = 32,
    S_ExtraInterface = 33,
    S_BlockBind = 40
};

struct PacketGoDirection
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    Position m_Pos;
    uint8 m_Oriection;

    PacketGoDirection() :
        m_PacketID(SMSG::S_UnitGoDirection) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, Position p_Pos, uint8 p_Orientation)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Pos.m_X << p_Pos.m_Y << p_Orientation;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
        m_Pos = p_Pos;
        m_Oriection = p_Orientation;
    }
};

struct PacketStopMovement
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    Position m_Pos;
    uint8 m_Oriection;

    PacketStopMovement() :
        m_PacketID(SMSG::S_UnitStopMovement) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, Position p_Pos, uint8 p_Orientation)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Pos.m_X << p_Pos.m_Y << p_Orientation;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
        m_Pos = p_Pos;
        m_Oriection = p_Orientation;
    }
};

struct PacketStartAttack
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    Position m_Pos;
    uint8 m_Oriection;

    PacketStartAttack() :
        m_PacketID(SMSG::S_UnitStartAttack) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, Position p_Pos, uint8 p_Orientation)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Pos.m_X << p_Pos.m_Y << p_Orientation;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
        m_Pos = p_Pos;
        m_Oriection = p_Orientation;
    }
};

struct PacketStopAttack
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;

    PacketStopAttack() :
        m_PacketID(SMSG::S_UnitStopAttack) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
    }
};

struct PacketUpdateOrientation
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    uint8 m_Orientation;

    PacketUpdateOrientation() :
        m_PacketID(SMSG::S_UnitUpdateOrientation) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, uint8 p_Orientation)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Orientation;
        m_Orientation = p_Orientation;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
    }
};

struct PacketUpdatePosition
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    uint16 m_MapID;
    uint32 m_PosX;
    uint32 m_PosY;
    uint8 m_Orientation;

    PacketUpdatePosition() :
        m_PacketID(SMSG::S_UnitUpdatePosition) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_MapID << p_PosX << p_PosY << p_Orientation;
        m_Orientation = p_Orientation;
        m_MapID = p_MapID;
        m_PosX = p_PosX;
        m_PosY = p_PosY;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
    }
};

struct PacketUnitCreate
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    Position m_Pos;
    uint8 m_Oriection;

    PacketUnitCreate() :
        m_PacketID(SMSG::S_UnitCreate) {}

    /// PLAYER / CREATURE
    void BuildPacket(uint8 p_TypeID, uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_Health, uint8 p_Mana, uint8 p_Alignment, int16 p_SkinID, uint8 p_SizeX, uint8 p_SizeY, uint8 p_Speed, uint16 p_MapID, Position p_Pos, uint8 p_Orientation, bool p_IsInMovement, bool p_IsAttacking)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Name << p_Level << p_Health << p_Mana << p_Alignment << p_SkinID << p_SizeX << p_SizeY << p_Speed << p_MapID << p_Pos.m_X << p_Pos.m_Y << p_Orientation << p_IsInMovement << p_IsAttacking;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
        m_Pos = p_Pos;
        m_Oriection = p_Orientation;
    }

    /// DYNAMICOBJECT
    void BuildPacket(uint8 p_TypeID, uint32 p_ID, std::string p_Name, int16 p_SkinID, uint8 p_SizeX, uint8 p_SizeY, uint8 p_Speed, uint16 p_MapID, Position p_Pos, uint8 p_Orientation, bool p_IsInMovement, bool p_IsBlocking)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Name << p_SkinID << p_SizeX << p_SizeY << p_Speed << p_MapID << p_Pos.m_X << p_Pos.m_Y << p_Orientation << p_IsInMovement << p_IsBlocking;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
        m_Pos = p_Pos;
        m_Oriection = p_Orientation;
    }
};

struct PacketUnitRemove
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;

    PacketUnitRemove() :
        m_PacketID(SMSG::S_UnitRemove) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
    }
};

struct PacketUpdateSkin
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    int16 m_Skin;

    PacketUpdateSkin() :
        m_PacketID(SMSG::S_UnitUpdateSkin) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, int16 p_Skin)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Skin;
        m_Skin = p_Skin;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
    }
};

struct PacketTalk
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    std::string m_TalkString;

    PacketTalk() :
        m_PacketID(SMSG::S_UnitTalk) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, std::string p_TalkString)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_TalkString;
        m_ID = p_ID;
        m_TypeID = p_TypeID;
        m_TalkString = p_TalkString;
    }
};

struct PacketSrvPlayerMsg
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    std::string m_Msg;

    PacketSrvPlayerMsg() :
        m_PacketID(SMSG::S_SrvPlayerMsg) {}

    void BuildPacket(const std::string & p_Msg)
    {
        m_Packet << m_PacketID << p_Msg;
        m_Msg = p_Msg;
    }
};

struct PacketLogDamage
{
	WorldPacket m_Packet;
	uint8 m_PacketID;
	uint8 m_TypeID;
	uint16 m_ID;
	int8 m_Damage;
	uint8 m_Result;

	PacketLogDamage() :
		m_PacketID(SMSG::S_LogDamage) {}

	void BuildPacket(uint8 p_TypeID, uint16 p_ID, sf::Int8 p_Damage, uint8 p_Result)
	{
		m_Packet << m_PacketID << p_TypeID << p_ID << p_Damage << p_Result;
		m_Damage = p_Damage;
		m_TypeID = p_TypeID;
		m_ID = p_ID;
		m_Result = p_Result;
	}
};

struct PacketSwitchMap
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint16 m_MapID;

    PacketSwitchMap() :
        m_PacketID(SMSG::S_SwitchMap) {}

    void BuildPacket(const uint16 & p_MapID, const std::string & p_MapFileName, const std::string & p_ChipsetFile, const std::string & p_MapName)
    {
        m_Packet << m_PacketID << p_MapID << p_MapFileName << p_ChipsetFile << p_MapName;
        m_MapID = p_MapID;
    }
};

struct PacketWarningMsg
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    std::string m_Msg;
    uint8 m_Type;

    PacketWarningMsg() :
        m_PacketID(SMSG::S_WarningMsg) {}

    void BuildPacket(const uint8 & p_Type, const std::string & p_Msg)
    {
        m_Packet << m_PacketID << p_Type << p_Msg;
        m_Msg = p_Msg;
        m_Type = p_Type;
    }
};

struct PacketUnitPlayVisual
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    uint8 m_VisualID;

    PacketUnitPlayVisual() :
        m_PacketID(SMSG::S_UnitPlayVisual) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, uint8 p_VisualID)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_VisualID;
        m_VisualID = p_VisualID;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
    }
};


struct PacketUnitIsInGroup
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    bool m_IsInGroup;

    PacketUnitIsInGroup() :
        m_PacketID(SMSG::S_UnitIsInGroup) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, bool p_IsInGroup)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_IsInGroup;
        m_IsInGroup = p_IsInGroup;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
    }
};


struct PacketKeyBoardBind
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeAction;
    uint8 m_Key;

    PacketKeyBoardBind() :
        m_PacketID(SMSG::S_KeyBoardBind) {}

    void BuildPacket(uint8 p_TypeAction, uint8 p_Key)
    {
        m_Packet << m_PacketID << p_TypeAction << p_Key;
        m_TypeAction = p_TypeAction;
        m_Key = p_Key;
    }
};

struct PacketKeyBoardBlock
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeAction;
    uint16 m_Time;

    PacketKeyBoardBlock() :
        m_PacketID(SMSG::S_BlockBind) {}

    void BuildPacket(uint8 p_TypeAction, uint16 p_Time)
    {
        m_Packet << m_PacketID << p_TypeAction << p_Time;
        m_TypeAction = p_TypeAction;
        m_Time = p_Time;
    }
};

struct PacketUnitCastBar
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    uint8 m_Time;

    PacketUnitCastBar() :
        m_PacketID(SMSG::S_CastBar) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, uint8 p_Time)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Time;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
        m_Time = p_Time;
    }
};

struct PacketUnitUpdateStat
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_TypeID;
    uint16 m_ID;
    uint8 m_TypeStat;
    uint16 m_StatNb;

    PacketUnitUpdateStat() :
        m_PacketID(SMSG::S_UnitUpdateStat) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, uint8 p_TypeStat, uint16 p_StatNb)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_TypeStat << p_StatNb;
        m_TypeID = p_TypeID;
        m_ID = p_ID;
        m_TypeStat = p_TypeStat;
        m_StatNb = p_StatNb;
    }
};

struct PacketLoadingPing
{
    WorldPacket m_Packet;
    uint8 m_PacketID;

    PacketLoadingPing() :
        m_PacketID(SMSG::S_LoadingPing) {}

    void BuildPacket()
    {
        m_Packet << m_PacketID;
    }
};

struct PacketExtraInterface
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    eExtraInterface m_ExtraInterface;
    bool m_Enable;

    PacketExtraInterface() :
        m_PacketID(SMSG::S_ExtraInterface) {}

    void BuildPacket(eExtraInterface p_ExtraInterface, bool p_Enable)
    {
        m_Packet << m_PacketID << (uint8)p_ExtraInterface << p_Enable;
    }
};
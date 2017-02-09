#pragma once
#include "WorldPacket.hpp"

enum CMSG : uint8
{
    C_Connexion = 1,
    C_UnitCreate = 10,
    C_UnitGoDirection = 20,
    C_UnitStopMovement = 21,
    C_UnitTalk = 23,
    C_UnitStartAttack = 24,
    C_UnitStopAttack = 25,
    C_UnitEventAction = 26
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
    S_UnitGoDirection = 20,
    S_UnitStopMovement = 21,
    S_UnitUpdatePosition = 22,
    S_UnitUpdateOrientation = 23,
    S_UnitTalk = 24,
    S_SrvPlayerMsg = 25,
    S_UnitStartAttack = 26,
    S_UnitStopAttack = 27,
    S_UnitUpdateSkin = 28,
    S_UnitPlayVisual = 30,
    S_BlockBind = 31
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

    void BuildPacket(uint8 p_TypeID, uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_Health, uint8 p_SkinID, uint16 p_MapID, Position p_Pos, uint8 p_Orientation, bool p_IsInMovement, bool p_IsAttacking)
    {
        m_Packet << m_PacketID << p_TypeID << p_ID << p_Name << p_Level << p_Health << p_SkinID << p_MapID << p_Pos.m_X << p_Pos.m_Y << p_Orientation << p_IsInMovement << p_IsAttacking;
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
    uint8 m_Skin;

    PacketUpdateSkin() :
        m_PacketID(SMSG::S_UnitUpdateSkin) {}

    void BuildPacket(uint8 p_TypeID, uint16 p_ID, uint8 p_Skin)
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

    void BuildPacket(const uint16 & p_MapID)
    {
        m_Packet << m_PacketID << p_MapID;
        m_MapID = p_MapID;
    }
};

struct PacketWarningMsg
{
    WorldPacket m_Packet;
    uint8 m_PacketID;
    uint8 m_MsgID;

    PacketWarningMsg() :
        m_PacketID(SMSG::S_WarningMsg) {}

    void BuildPacket(const uint8 & p_MsgID)
    {
        m_Packet << m_PacketID << p_MsgID;
        m_MsgID = p_MsgID;
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
    uint32 m_Time;

    PacketKeyBoardBlock() :
        m_PacketID(SMSG::S_BlockBind) {}

    void BuildPacket(uint8 p_TypeAction, uint32 p_Time)
    {
        m_Packet << m_PacketID << p_TypeAction << p_Time;
        m_TypeAction = p_TypeAction;
        m_Time = p_Time;
    }
};
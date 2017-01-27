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
    S_UnitCreate = 11,
    S_UnitRemove = 12,
    S_UnitUpdateResource = 13,
    S_PlayerUpdateXp = 14,
	S_LogDamage = 15,
    S_UnitGoDirection = 20,
    S_UnitStopMovement = 21,
    S_UnitUpdatePosition = 22,
    S_UnitUpdateOrientation = 23,
    S_UnitTalk = 24,
    S_SrvPlayerMsg = 25,
    S_UnitStartAttack = 26,
    S_UnitStopAttack = 27,
    S_UnitUpdateSkin = 28
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
	uint8 m_Damage;
	bool m_Miss;

	PacketLogDamage() :
		m_PacketID(SMSG::S_LogDamage) {}

	void BuildPacket(uint8 p_TypeID, uint16 p_ID, uint8 p_Damage, bool p_Miss)
	{
		m_Packet << m_PacketID << p_TypeID << p_ID << p_Damage << p_Miss;
		m_Damage = p_Damage;
		m_TypeID = p_TypeID;
		m_ID = p_ID;
		m_Miss = p_Miss;
	}
};
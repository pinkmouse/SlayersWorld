#pragma once

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

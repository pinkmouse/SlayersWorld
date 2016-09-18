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
};

enum SMSG : uint8
{
    S_Connexion = 1,
    S_PlayerCreate = 10,
    S_UnitCreate = 11,
    S_UnitRemove = 12,
    S_PlayerUpdateLife = 13,
    S_UnitGoDirection = 20,
    S_UnitStopMovement = 21,
    S_UnitUpdatePosition = 22,
    S_UnitTalk = 23,
    S_UnitStartAttack = 24,
    S_UnitStopAttack = 25,
};
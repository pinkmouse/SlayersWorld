#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "WorldPacket.hpp"
#include "../Define.hpp"
#include "../Entities/Player.hpp"

class WorldSocket : public sf::TcpSocket
{
public:
	WorldSocket();
	~WorldSocket();

    void SendPlayerCreate(uint32, std::string, uint8, uint8, uint8, uint8, int16, uint16, const std::string &, const std::string &, const std::string &, uint32, uint32, uint8);
    //void SendUnitCreate(uint8, uint32, std::string, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint8, uint16, Position, uint8, bool, bool, bool, bool);
    void SendUnitCreate(Unit*, bool);
    void SendUnitGoDirectionToSet(uint8, uint16, const Position &, uint8);
    void SendUnitStopMovement(uint8, uint16, const Position &, uint8);
    void SendUnitStartAttack(uint8, uint16);
    void SendUnitStopAttack(uint8, uint16);
    void SendUpdateXpPct(float);
    void SendUpdateUnitResource(uint8, uint16, uint8, uint8);
    void SendUnitTalk(uint8, uint16, const std::string &);
	void SendLogDamage(uint8, uint16, DamageInfo);
	void SendAuthResponse(uint8);
    void SendPacket(WorldPacket);
    void SendToSet(WorldPacket, bool p_ExcludePlayer = false, bool p_OnlyGroup = false);
    void SendSwitchMap(uint16, const std::string &, const std::string &, const std::string &);
    void SendKeyBoardBind(eKeyBoardAction, uint8);
    void SendKeyBoardBind(std::map< eKeyBoardAction, uint8 >*);
    void SendUnitIsInGroup(uint8, uint32, bool);
    void SendUnitMount(const uint8 &, const uint32 &, const int16 &);
    void SendTitles(std::map<uint16, Title*>*);
    void SendSkins(std::map<uint16, Skin*>*);
    void SendSpells(std::map<uint16, uint64>*);
    void SendItems(std::map<uint8, Item*>*);
    void SendCurrencies(std::map<eTypeCurrency, uint16>*);
    void SendBagSize(const uint8 &);
    void SendEquipments(std::map<eTypeEquipment, Item*>*);

    Player* GetPlayer();
    void SetPlayer(Player*);

    bool IsDisonnected() const;
    void Kick();

private:
    Player* m_Player;
    bool m_Disconnected;
};


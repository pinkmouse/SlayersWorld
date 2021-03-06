#pragma once
#include  <SFML/Window/Keyboard.hpp>
#include "../System/Quest/Quest.hpp"
#include "../System/Reward/Reward.hpp"
#include "Unit.hpp"
#include "../System/Item/Item.hpp"
#include <map>

class WorldSocket;

class Player : public Unit
{
public:
    Player(uint32, int32, std::string, uint8, eClass, uint8, uint8, uint8, int16, uint16, uint32, uint32, Orientation, uint32, eAccessType);
    ~Player();

    /* BASIC */
    WorldSocket* GetSession() const;
    void SetSession(WorldSocket*);
    uint32 GetAccountID() const;
    eAccessType GetAccessType() const;
    void HasBeenInitialize();
    void Respawn();
    void SendMsg(const std::string &, const eTextColor & p_Color = eTextColor::TextColorWhite, const eTextStyle & p_Style = eTextStyle::TextStyleRegular);
    void SendSimpleQuestion(const uint16 &, const std::string &);
    void Save();
    void SetInLoading(bool);
    bool GetInLoading() const;
    void SetPointsSet(const PointsSet &);
    bool AddPointsStat(eStats, uint8);
    bool SubPointsStat(eStats, uint8);
    void SetLevel(const uint8 &);
    void ActionFrom(Player*);
    void ParseStringWithTag(std::string &);
    void SetClass(eClass);
    eClass GetClass() const;
    void AddQuestionInProgress(uint16, Unit*, std::vector<uint16>);
    void RemoveQuestionInProgress(uint16);
    void SetPlayerMod(const ePlayerMod &);
    std::pair<Unit*, uint16> GetGossipForQuestion(const uint16 &, const uint8 &);
    ePlayerMod GetPlayerMod() const;

    /* REWARD */
    void AddRewardList(std::vector<SubReward*>);
    void RewardItem(const uint16 &, const uint8 &);
    void RewardCurrency(const eTypeCurrency &, const uint16 &);

    /* TITLES */
    void AddTitle(const uint16 &, Title*);
    std::map < uint16, Title*>* GetTitles();
    void ChangeActiveTitle(const uint16 &, bool p_Send = true);
    void RemoveActiveTitle();
    std::string GetNameWithTitle();
    int32 GetActiveTitleID() const;
    void LearnTitle(const uint16 &, const bool & p_New = false);

    /* SKINS */
    void AddSkinToCollection(const uint16 &, Skin*);
    std::map < uint16, Skin*>* GetSkinsCollection();
    bool HasSkin(const uint16 &);
    void LearnSkin(const uint16 &, const bool & p_New = false);

    /* CURRENCY */
    void UpdateCurrency(const eTypeCurrency &, const uint16 &, const bool & p_Send = false);
    uint16 GetCurrency(const eTypeCurrency &);
    std::map< eTypeCurrency, uint16>* GetCurrencies();

    /* ITEMS */
    void AddItem(const uint8 &, Item*, bool p_New = false);
    bool AddItemOnAvailableSlot(Item*);
    bool ConsumeItemFromBag(const uint8 &);
    std::map< uint8, Item*>* GetItems();
    Item* GetItem(const uint8 &);
    void RemoveItemFromBag(const uint8 &, const bool & p_Delete = true);
    void ActionItem(const uint8 &);
    void UnstackItem(const uint8 &);
    uint8 StackItem(const uint8 &, const uint8 &);
    void SetMaxBagSlot(const uint16 &);
    uint8 GetBagSize() const;
    int16 CanCanBeStackOnBagSlot(Item*);
    void SellItem(const uint8 &);
    void OpenSeller();

    /* EQUIPMENT */
    void AddEquipment(const eTypeEquipment &, Item*, bool p_New = false);
    std::map< eTypeEquipment, Item*>* GetEquipments();
    Item* GetEquipment(const eTypeEquipment &);
    void RemoveEquipment(const eTypeEquipment &);
    void UnEquip(const eTypeEquipment &);

    /* RESOURCES */
    uint32 GetXp() const;
    void SetXp(uint32);
    void SetResourceNb(eResourceType, uint8);
    void AddResourceNb(eResourceType, uint8);

    /* UPDATE */
    void Update(sf::Time);
    void UpdateNewSquares(uint16, uint16, bool p_UpdateAll = false);

    /* QUEST */
    void UpdateQuests();
    void AddQuest(Quest*, bool p_New = true);
    void RemoveQuest(uint16);
    Quest* GetQuest(uint16) const;
    std::map< uint16, Quest* >* GetQuestList();
    bool CheckQuestObjective(eObjectifType, int32);
    bool HasQuestInProgress(uint16);
    void ValidateQuest(Quest*);

    /* KEYBOARD */
    void AddKeyBoardBind(eKeyBoardAction, uint8);
    int16 GetKeyBoardBind(eKeyBoardAction);
    void AddSpellBindToKey(uint16, uint8);
    void RemoveSpellBindToKey(uint16);
    int32 GetBindSpell(uint16);
    std::map< eKeyBoardAction, uint8 >* GetKeyBoardBinds();
    int32 GetSpellOnBind(uint8);

    /* SPELL */
    void CastSpell(uint16);
    void AddSpellCooldown(uint16, uint64);
    void LearnClass(eClass);
    void LearnSpell(uint16);
    void UnlearnSpell(uint16);
    void UnlearnAllSpell();

    /* ZONE */
    void EnterInZone(Zone*);
    void OutOfZone(Zone*);

    /* GROUP */
    void LeaveGroup(const std::string &);
    void EnterInGroup(const std::string &);
    void UnitLeaveGroup(Unit*, const std::string &);
    void UnitEnterInGroup(Unit*, const std::string &);

    /* COMMANDS */
    bool CheckCommand(const std::string &);
    void EventAction(eKeyBoardAction);
    void InitializeCommands();
    bool HandleCommandSkin(std::vector<std::string>);
    bool HandleCommandWhere(std::vector<std::string>);
    bool HandleCommandLevel(std::vector<std::string>);
    bool HandleCommandWisp(std::vector<std::string>);
	bool HandleCommandAddPoint(std::vector<std::string>);
    bool HandleCommandClass(std::vector<std::string>);
    bool HandleSave(std::vector<std::string>);
    bool HandleCommandCreature(std::vector<std::string>);
    bool HandleCommandWho(std::vector<std::string>);
    bool HandleCommandTeleport(std::vector<std::string>);
    bool HandleCommandSpeed(std::vector<std::string>);
    bool HandleCommandQuests(std::vector<std::string>);
    bool HandleCommandJoin(std::vector<std::string>);
    bool HandleCommandLeave(std::vector<std::string>);
    bool HandleCommandEmote(std::vector<std::string>);
    bool HandleCommandAnnounce(std::vector<std::string>);
    bool HandleCommandServer(std::vector<std::string>);
    bool HandleCommandBG(std::vector<std::string>);
    bool HandleCommandSummonPlayer(std::vector<std::string>);
    bool HandleCommandGroupWisp(std::vector<std::string>);    
    bool HandleCommandTeamWisp(std::vector<std::string>);
    bool HandleRegen(std::vector<std::string>);
    bool HandleKick(std::vector<std::string>);
    bool HandleMount(std::vector<std::string>);
    bool HandleDismount(std::vector<std::string>);
    bool HandleServerRestart(std::vector<std::string>);
    bool HandlePlayerInfo(std::vector<std::string>);
    bool HandleBanIP(std::vector<std::string>);
    bool HandleBanAccount(std::vector<std::string>);
    bool HandleBind(std::vector<std::string>);
    bool handleGodMod(std::vector<std::string>);
    bool handleCast(std::vector<std::string>);
    bool HandleTest(std::vector<std::string>);

private:
    uint16 m_AccountID;
    WorldSocket* m_Session;
    ePlayerMod m_Mod;

    uint8 m_Alignment;
    uint32 m_Xp;
    bool m_Initilize;
    bool m_InLoading;
    eAccessType m_AccessType;
    eClass m_Class;
    std::map< uint16, Quest* > m_Quests;
    int32 m_ActiveTitle;
    uint8 m_BagSlots;

    std::map < uint16, std::pair<Unit*, std::vector<uint16> > >     m_QuestionInProgress;
    typedef bool(Player::*m_Func)(std::vector<std::string>);
    std::map < std::string, std::pair < eAccessType,  m_Func > >    m_CmdHandleMap;
    std::map < eKeyBoardAction, uint8 >                             m_KeyBoardBinds;
    std::map < uint16, uint8 >                                      m_SpellsBindToKey;
    std::map < uint16, Title* >                                     m_Titles;
    std::map < uint16, Skin* >                                      m_Skins;
    std::map < uint8, Item* >                                       m_Items;
    std::map < eTypeEquipment, Item* >                              m_Equipment;
    std::map < eTypeCurrency, uint16 >                              m_Currencies;
};


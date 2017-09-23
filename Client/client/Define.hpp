#pragma once

#include <cstdio>
#include <SFML/Graphics/Text.hpp>
#include "System/SWType.hpp"

#define CONF_FILE "config.wtf"
//#define IP_SERVER "127.0.0.1"
//#define IP_SERVER "51.255.165.231"
#define PORT_SERVER 1234

/// GRAPHICS
#define X_WINDOW 640
#define Y_WINDOW 480
#define ZOOM_FACTOR 0.5f

/// TILES
#define MAX_TILE_LEVEL 6
#define TILE_SIZE 16
#define TILE_FOLDER "img/tiles/"
#define GRID_SIZE 12

/// SKIN
#define MAX_SKIN_IMG 91
#define MAX_SKIN_MOUNT_IMG 2
#define MAX_SKIN_ANMATIONUNIT_IMG 1
#define SKIN_ZOOM_FACTOR_DEFAULT 1.0f
#define UPDATE_OPACITY_TIME 250
#define SKINS_FOLDER "img/skins/"
#define SKINS_MOUNT_FOLDER "img/skins/mount/"
#define SKINS_ANIMATIONUNIT_FOLDER "img/skins/animationUnit/"
#define SKIN_SIZE_X 24
#define SKIN_SIZE_Y 32
#define SKIN_MOUNT_SIZE_X 48
#define SKIN_MOUNT_SIZE_Y 33
#define SKIN_OFFSET_SIZE_X 14
#define MAX_MOVEMENT_POSITION 3
#define UPDATE_TIME_MOVEMENT 200
#define DEATH_OPACITY 100
#define MAX_OPACITY 255

/// GOB
#define MAX_GOB_IMG 23
#define GOBS_SKIN_FOLDER "img/gobs/"

/// SPELL
#define MAX_SPELL_IMG 13
#define SPELLS_FOLDER "img/spells/"
#define MAX_VISUAL_IMG_X 9
#define MAX_VISUAL_IMG_Y 4

#define MAX_GOB_IMG_X 3
#define MAX_GOB_IMG_Y 4

/// UNIT
#define TALK_TIME_SHOW 5000

/// CHATBAR
#define MAX_LETTER_STR 70
#define SIZE_TALK_TEXT_SIZE 200

/// FONT
#define FONT_FOLDER "font/"
#define SIZE_NAME_FONT 20
#define SIZE_TALK_FONT 20

/// INTERFACE
#define IMG_FOLDER "img/window/"
#define FONT_FIELD_SIZE 128
#define FONT_FIELD_BORDER_SIZE 64
#define SIZE_FILED_TALK_Y 24
#define FLASK_SIZE_X 16
#define FLASK_SIZE_Y 72
#define LIFE_BAR_X 74
#define LIFE_BAR_Y 13
#define FLASK_OFFSET_TOP 22
#define FLASK_OFFSET_BOTTOM 2
#define FLASK_SCALE 2.0f

#define XP_BAR_SIZE_X 320
#define XP_BAR_SIZE_Y 12
#define XP_BAR_SCALE 2.0f

#define MIN_HISTORY_LINE 5
#define MAX_HISTORY_LINE 19

#define MAX_HISTORY_LOG_TIME 2000000
#define MAX_WARNING_LOG_TIME 5000000
#define MAX_WARNING_LOG_TIME_BETWEEN_SAME 2000000

#define MENU_COLUMN_SIZE 100
#define MENU_ROW_SIZE 20

/// EVENT
#define MAX_KEY_SAVE 4

/// MAP
#define MAP_FOLDER "map/"
#define NAME_WINDOW "Slayers World"

/// GENERAL
#define STEP_SIZE 12 /// In pixel
#define IN_MILLISECOND 1000

enum Orientation
{
    Up      = 0,
    Right   = 1,
    Down    = 2,
    Left    = 3,
    MAX     = 4
};

enum TypeWorldObject
{
    UNIT = 0,
    CASE = 1
};

enum TypeUnit
{
    CREATURE = 0,
    PLAYER,
    ANIMATIONUNIT,
    AREATRIGGER,
    GAMEOBJECT
};

enum eTypeWarningMsg
{
    Red = 0,
    Yellow,
    Top,
    BigMsg,
    WarningReward
};

struct Position
{
    uint32 x;
    uint32 y;
};

enum DamageResult
{
    Success = 0,
    Miss = 1
};

enum eStats
{
    Free = 0,
    Dexterity,
    Force,
    Stamina,
    Speed,
    Level,
    MaxStat
};

enum eTextColor
{
    TextColorWhite = 0,
    TextColorGreen = 1,
    TextColorRed = 2,
    TextColorBlue = 3,
    TextColorYellow = 4
};

enum eTextStyle
{
    TextStyleRegular = 0,
    TextStyleBold = 1,
    TextStyleItalic = 2,
    TextStyleUnderLine = 3,
    TextStyleStrikeThrough = 4,
};

enum eItemType
{
    ITEM_USELESS = 0,
    ITEM_LAUNCHSPELL = 1,
    ITEM_EQUIPMENT = 2,
    ITEM_QUEST = 3,
    ITEM_CONSUMABLE = 4,
    ITEM_MAX = 5
};

enum eItemRareLevel
{
    ITEM_RARE1 = 0,
    ITEM_RARE2 = 1,
    ITEM_RARE3 = 2,
    ITEM_RARE4 = 3,
    ITEM_RARE5 = 4,
    ITEM_MAX_RARE = 5
};

enum eTypeWeapon
{
    WEAPON_ONE_HAND = 0,
    WEAPON_TWO_HAND = 1
};

enum eTypeEquipment
{
    EQUIP_HEAD = 0,
    EQUIP_SHOLDER = 1,
    EQUIP_BODY = 2,
    EQUIP_HAND = 3,
    EQUIP_FOOT = 4,
    EQUIP_MAIN_HAND = 5,
    EQUIP_SECOND_HAND = 6,
    EQUIP_MAX = 7
};

enum eExtraInterface
{
    eBattelGroundUI = 0
};

struct DamageInfo
{
	int8 m_Damage;
	DamageResult m_Result;

	DamageInfo() :
		m_Damage(0), m_Result(DamageResult::Success) {}

	DamageInfo(int8 p_Damage, DamageResult p_Result) :
		m_Damage(p_Damage), m_Result(p_Result) {}

	DamageInfo(const DamageInfo & p_DamageInfo) :
		m_Damage(p_DamageInfo.m_Damage), m_Result(p_DamageInfo.m_Result) {}
};

enum eVisualType
{
    VisualSkin = 0,
    VisualSpell = 1,
    VisualGob = 2,
    VisualSkinMount = 3,
    VisualAnimationUnit = 4
};

enum eTimeSpellAttribute
{
    CooldownAttribute = 0,
    CastimeAttribute = 1
};

enum eTypeCurrency
{
    CURRENCY_BASISC = 0,
    CURRENCY_HONNOR = 1
};

enum eMenuType
{
    MainMenu = 0,
    QuestMenu,
    StatsMenu,
    TitlesMenu,
    WardrobeMenu,
    SpellsMenu,
    BagMenu,
    EquipmentMenu
};

enum eKeyBoardAction
{
    NoneAction = 0,
    KeyBoardDown,
    KeyBoardUp,
    KeyBoardLeft,
    KeyBoardRight,
    KeyBoardAction,
    KeyBoardEnter,
    KeyBoardHistoryMsg,
    KeyBoardAutoAttack,
    KeyBoardSpell0,
    KeyBoardSpell1,
    KeyEscape,
    MaxKeyBoard
};

enum eActionType
{
    Go = 0,
    Attack = 1,
    Stop = 2,
    StopAttack = 4
};

enum eResourceType
{
    None = 0,
    Health = 1,
    Mana = 2,
    Alignment = 3
};

enum eWarningMsg
{
   NotEnoughMana = 0,
   InCooldown
};

struct SWText
{
    std::string m_Str;
    eTextColor m_TextColor;
    eTextStyle m_Style;

    SWText(const std::string & p_Str) :
        m_Str(p_Str), m_TextColor(eTextColor::TextColorWhite), m_Style(eTextStyle::TextStyleRegular) {}

    SWText(const std::string & p_Str, const eTextColor & p_TextColor, const eTextStyle & p_Style) :
        m_Str(p_Str), m_TextColor(p_TextColor), m_Style(p_Style) {}
};


struct Item
{
    eItemType m_Type;
    uint8 m_SubType;
    std::string m_Name;
    eItemRareLevel m_RareLevel;
    uint8 m_Level;
    uint8 m_StackNb;
    std::vector<int32> m_Datas;

    Item() :
        m_Type(eItemType::ITEM_USELESS), m_SubType(0), m_Name(""), m_RareLevel(eItemRareLevel::ITEM_RARE1), m_Level(0), m_StackNb(1) {}

    /*Item(Item & p_Item) :
        m_Type(p_Item.m_Type), m_SubType(p_Item.m_SubType), m_Name(p_Item.m_Name), m_RareLevel(p_Item.m_RareLevel), m_Level(p_Item.m_Level), m_StackNb(p_Item.m_StackNb) 
    {
        for (uint8 i = 0; i < 4; i++)
        {
            AddData(p_Item.GetData(i));
        }
    }*/

    Item(const eItemType & p_Type, const uint8 & p_SubType, const std::string & p_Name, const eItemRareLevel & p_RareLevel, const uint8 p_Level, const uint8 p_StackNb) :
        m_Type(p_Type), m_SubType(p_SubType), m_Name(p_Name), m_RareLevel(p_RareLevel), m_Level(p_Level), m_StackNb(p_StackNb) {}


    void AddData(const int32 & p_Data)
    {
        m_Datas.push_back(p_Data);
    }

    int32 GetData(const uint8 & p_DataNB)
    {
        if (p_DataNB >= m_Datas.size())
            return -1;
        return m_Datas[p_DataNB];
    }
};
#pragma once
#include "DynamicObject.hpp"

class GameObjectTemplate
{
public:
    GameObjectTemplate();
    GameObjectTemplate(uint16, int32, int32, eGameObjectTemplate, int16);
    ~GameObjectTemplate();
    void SetData(uint8, uint32);
    uint32 GetData(uint8);
    int32 GetDuration() const;
    int32 GetRespawnTime() const;
    int16 GetSkinID() const;
    eGameObjectTemplate GetType() const;
    uint16 GetID() const;

private:
    uint16 m_Id;
    std::map<uint8, uint32> m_Datas;
    eGameObjectTemplate m_Type;
    int16 m_SkinID;
    int32 m_Duration;
    int32 m_RespawnTime;
};

class GameObject : public DynamicObject
{
public:
    /* BASIC */
    GameObject(uint16, Map*, uint32, uint32, GameObjectTemplate*);
    ~GameObject();
    void Update(sf::Time);
    void UnitEnterInCase(Unit*);
    bool CanBeWalk();
    bool IsBlocking();

private:
    /* BASIC */
    GameObjectTemplate* m_GobTemplate;
};


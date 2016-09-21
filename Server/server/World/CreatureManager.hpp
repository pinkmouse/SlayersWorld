#pragma once
#include "../Define.hpp"
#include <map>

class CreatureManager
{
public:
    CreatureManager();
    ~CreatureManager();
    void AddCreatureTemplate(const CreatureTemplate &);
    CreatureTemplate GetCreatureTemplate(uint32);

private:
    std::map<uint16, CreatureTemplate> m_CreatureTemplateMap;
};


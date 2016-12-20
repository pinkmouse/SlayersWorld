#pragma once
#include "../Define.hpp"
#include "../Entities/WorldObject.hpp"

class Case : public WorldObject
{
public:
	Case(uint16, uint16, uint16);
	~Case();
	void SetBlock(bool);
    bool IsBlocking() const;

private:
	bool m_Block;
	uint16 m_ID;
};


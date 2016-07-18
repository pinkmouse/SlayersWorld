#pragma once

#include <winsock.h>
#include <mysql.h>
#include <string>
#include "../Define.hpp"
#include "../Entities/Player.hpp"

class SqlManager
{
public:
	SqlManager();
	~SqlManager();
	bool Initialize(std::string, std::string, std::string, std::string, std::string);

    int32 GetIDLogin(std::string, std::string);
    Player* GetNewPlayer(uint32);

private:
	MYSQL m_Mysql;
};


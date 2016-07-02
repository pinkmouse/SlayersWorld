#pragma once

#include <winsock.h>
#include <mysql.h>
#include <string>

class SqlManager
{
public:
	SqlManager();
	~SqlManager();
	bool Initialize(std::string, std::string, std::string, std::string, std::string);

private:
	MYSQL m_Mysql;
};


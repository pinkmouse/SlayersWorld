#include "SqlManager.hpp"
#include "../Define.hpp"

SqlManager::SqlManager()
{
	mysql_init(&m_Mysql);
}


SqlManager::~SqlManager()
{
}

bool SqlManager::Initialize(std::string p_Host, std::string p_User, std::string p_PassWord, std::string p_DB, std::string p_Port)
{
	if (!mysql_real_connect(&m_Mysql, p_Host.c_str(), p_User.c_str(), p_PassWord.c_str(), p_DB.c_str(), std::stoi(p_Port), NULL, 0))
		return false;
	return true;
}
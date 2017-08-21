#pragma once

#include "WebHook.hpp"
#include "../Define.hpp"
#include "../Global.hpp"

static void SWExit(int p_Code)
{
    WebHook::sendMsg(g_Config->GetValue("WebhookUrl"), "Serveur" + g_Config->GetValue("ServerName") + " stop!");
    exit(p_Code);
}
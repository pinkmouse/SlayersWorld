#ifdef __linux__
    #include <execinfo.h>
    #include <unistd.h>
#endif

#include "Map/Map.hpp"
#include "World/World.hpp"
#include "System/WebHook.hpp"
#include <csignal>
#include <stdlib.h>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

ConfigHandler* g_Config;
SqlManager* g_SqlManager;
LevelManager* g_LevelManager;
MapManager* g_MapManager;
SpellManager* g_SpellManager;
QuestManager* g_QuestManager;
GroupManager* g_GroupManager;

void handler_segfault(int sig) {
    void *l_Array[20];
    size_t size = 0;

#ifdef __linux__
    // get void*'s for all entries on the stack
    size = backtrace(l_Array, 20);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(l_Array, size, STDERR_FILENO);
    WebHook::sendMsg(g_Config->GetValue("WebhookUrl"), "Serveur " + g_Config->GetValue("ServerName") + " Crash !");
    std::string l_FinalStr = "";
    for (uint8 i = 0; i < size; i++)
    {
        if (l_Array[i] != NULL)
        {
            char* l_Str = static_cast<char*>(l_Array[i]);
            printf("Debug [%s]\n", l_Str);
            l_FinalStr += std::string(l_Str) + " ";
        }
    }
    WebHook::sendMsg(g_Config->GetValue("WebhookUrl"), l_FinalStr);
#endif
    exit(1);
}

int main()
{
    Log("Starting Slayers World...");
    signal(SIGSEGV, handler_segfault);
	World* l_World = new World();
	l_World->Run();
	return 0;
}
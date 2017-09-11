#ifdef __linux__
    #include <execinfo.h>
    #include <unistd.h>
#endif

#define MAX_SIZE_BACKTRACE 20
#include "Map/Map.hpp"
#include "World/World.hpp"
#include "Define.hpp"
#include "System/SWExit.hpp"
#include <csignal>
#include <stdlib.h>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

Skins            g_Skins;
Titles           g_Titles;
ConfigHandler    *g_Config;
SqlManager       *g_SqlManager;
LevelManager     *g_LevelManager;
MapManager       *g_MapManager;
SpellManager     *g_SpellManager;
QuestManager     *g_QuestManager;
GroupManager     *g_GroupManager;
ItemManager      *g_ItemManager;

void handler_segfault(int sig) {

#ifdef __linux__
    void *l_Array[MAX_SIZE_BACKTRACE];
    char **l_StrArray;
    size_t size = 0;

    // get void*'s for all entries on the stack
    size = backtrace(l_Array, MAX_SIZE_BACKTRACE);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    //backtrace_symbols_fd(l_Array, size, STDERR_FILENO);
    l_StrArray = backtrace_symbols(l_Array, size);
    WebHook::sendMsg(g_Config->GetValue("WebhookUrl"), "Serveur " + g_Config->GetValue("ServerName") + " Crash !");
    if (l_StrArray == NULL) {
        perror("backtrace_symbols");
        SWExit(EXIT_FAILURE);
    }
    std::string l_TotalBuffer = "==== BACKTRACE ===\\n";
    for (uint16 j = 0; j < size; j++) {
        printf("%s\n", l_StrArray[j]);
        l_TotalBuffer += (std::string(l_StrArray[j]) + "\\n");
    }
    WebHook::sendMsg(g_Config->GetValue("WebhookUrl"), l_TotalBuffer);
#endif
    SWExit(1);
}

int main()
{
    Log("Starting Slayers World...");
    signal(SIGSEGV, handler_segfault);
	World* l_World = new World();
	l_World->Run();
    SWExit(1);
	return 0;
}
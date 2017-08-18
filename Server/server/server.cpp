#ifdef __linux__
    #include <execinfo.h>
    #include <unistd.h>
#endif

#include "Map/Map.hpp"
#include "World/World.hpp"
#include "System\WebHook.hpp"
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
    void *array[20];
    size_t size = 0;

#ifdef __linux__
    // get void*'s for all entries on the stack
    size = backtrace(array, 20);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    WebHook::sendMsg(g_Config->GetValue("WebhookUrl"), "Serveur Crash !");
    WebHook::sendMsg(g_Config->GetValue("WebhookUrl"), static_cast<std::string*>(array));
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
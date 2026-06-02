#include <process.h>
#include "client.h"
#include "discord.h"
#include "query.h"
#include "http.h"

static void process(void*)
{
    SAMP::ServerData data;
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    Discord::initialize();

    if (SAMP::readServerData(GetCommandLine(), data)) {
        auto start = std::time(0);

        if (data.connect == SAMP::SAMP_CONNECT_SERVER) {
            SAMP::Query query(data.address, std::stoi(data.port));

            while (true) {
                SAMP::Query::Information information;

                if (query.info(information)) {
                    std::string topRow = data.address + ":" + data.port + " (" +
                        std::to_string(information.basic.players) + " / " +
                        std::to_string(information.basic.maxPlayers) + ")";

                    std::string bottomRow = reinterpret_cast<const char*>(u8"Имя: ") + data.username;

                    Discord::update(
                        start,
                        bottomRow,
                        topRow,
                        "walker",
                        bottomRow,
                        ""
                    );

                    Sleep(15000 - QUERY_DEFAULT_TIMEOUT * 2);
                }
                else {
                    std::string topRow = data.address + ":" + data.port + " (0 / 0)";
                    std::string bottomRow = reinterpret_cast<const char*>(u8"Подключение...");
                    Discord::update(start, bottomRow, topRow, "walker", data.username, "");
                    Sleep(5000);
                }
            }
        }
        else if (data.connect == SAMP::SAMP_CONNECT_DEBUG) {
            while (true) {
                std::string topRow = "localhost:7777 (1 / 1)";
                std::string bottomRow = reinterpret_cast<const char*>(u8"Имя: ") + data.username;
                Discord::update(start, bottomRow, topRow, "walker", data.username, "");
                Sleep(15000);
            }
        }
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(module);
        _beginthread(&process, 0, nullptr);
        break;
    }
    case DLL_PROCESS_DETACH: {
        WSACleanup();
        break;
    }
    }
    return TRUE;
}
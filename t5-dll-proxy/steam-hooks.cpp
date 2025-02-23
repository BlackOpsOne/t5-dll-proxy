#include "pch.h"

#include "steam-hooks.hpp"

HMODULE h_steam_api = nullptr;
std::map<std::string, FARPROC> steam_functions;

typedef BOOL(*SteamAPI_Init_t)();
SteamAPI_Init_t o_SteamAPI_Init = nullptr;

#define LOAD_FUNC(func) steam_functions[#func] = GetProcAddress(h_steam_api, #func)
#define EXPORT_FUNC(func) \
    extern "C" __declspec(dllexport) void func() { \
        auto fn = steam_functions[#func]; \
        if (fn) reinterpret_cast<void(*)()>(fn)(); \
    }

// export the functions that bo1 imports from steam_api.dll
// we only have custom calls for SteamAPI_Init so we just call the originals
EXPORT_FUNC(SteamAPI_RegisterCallResult)
EXPORT_FUNC(SteamAPI_Shutdown)
EXPORT_FUNC(SteamAPI_UnregisterCallback)
EXPORT_FUNC(SteamAPI_UnregisterCallResult)
EXPORT_FUNC(SteamAPI_RunCallbacks)
EXPORT_FUNC(SteamAPI_RestartAppIfNecessary)
EXPORT_FUNC(SteamApps)
EXPORT_FUNC(SteamGameServer)
EXPORT_FUNC(SteamGameServer_RunCallbacks)
EXPORT_FUNC(SteamMasterServerUpdater)
EXPORT_FUNC(SteamGameServer_Init)
EXPORT_FUNC(SteamGameServer_Shutdown)
EXPORT_FUNC(SteamNetworking)
EXPORT_FUNC(SteamUserStats)
EXPORT_FUNC(SteamAPI_RegisterCallback)
EXPORT_FUNC(SteamUtils)
EXPORT_FUNC(SteamUser)
EXPORT_FUNC(SteamFriends)

// SteamAPI_Init hook
extern "C" __declspec(dllexport) BOOL SteamAPI_Init() {
    // call our message window here so we know it worked
    MessageBoxA(nullptr, "SteamAPI_Init called by the game!", "Hook", MB_ICONINFORMATION);

    if (o_SteamAPI_Init) {
        return o_SteamAPI_Init(); // call original SteamAPI_Init
    }

    return FALSE;
}

namespace steam_hooks {
    BOOL Initialize() {
        h_steam_api = LoadLibraryA("o_steam_api.dll");
        if (!h_steam_api) {
            MessageBoxA(nullptr, "Failed to load o_steam_api.dll", "Error", MB_ICONERROR);
            return FALSE;
        }

        // load the functions from the original
        LOAD_FUNC(SteamAPI_RegisterCallResult);
        LOAD_FUNC(SteamAPI_Shutdown);
        LOAD_FUNC(SteamAPI_UnregisterCallback);
        LOAD_FUNC(SteamAPI_UnregisterCallResult);
        LOAD_FUNC(SteamAPI_RunCallbacks);
        LOAD_FUNC(SteamAPI_RestartAppIfNecessary);

        // special handling for steam api init calls
        // this is what we want to do on the game
        // for now we just have an example message window
        o_SteamAPI_Init = (SteamAPI_Init_t)GetProcAddress(h_steam_api, "SteamAPI_Init");

        // continue loading the original functions
        LOAD_FUNC(SteamApps);
        LOAD_FUNC(SteamGameServer);
        LOAD_FUNC(SteamGameServer_RunCallbacks);
        LOAD_FUNC(SteamMasterServerUpdater);
        LOAD_FUNC(SteamGameServer_Init);
        LOAD_FUNC(SteamGameServer_Shutdown);
        LOAD_FUNC(SteamNetworking);
        LOAD_FUNC(SteamUserStats);
        LOAD_FUNC(SteamAPI_RegisterCallback);
        LOAD_FUNC(SteamUtils);
        LOAD_FUNC(SteamUser);
        LOAD_FUNC(SteamFriends);

        return TRUE;
    }
}
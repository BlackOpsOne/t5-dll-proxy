#include "pch.h"
#include "steam-hooks.hpp"

HMODULE h_steam_api = nullptr;
std::map<std::string, FARPROC> steam_functions;

// type definitions for each of the functions
typedef BOOL(*SteamAPI_Init_t)();
typedef void(*SteamAPI_RegisterCallResult_t)(void);
typedef void(*SteamAPI_Shutdown_t)(void);
typedef void(*SteamAPI_UnregisterCallback_t)(void);
typedef void(*SteamAPI_UnregisterCallResult_t)(void);
typedef void(*SteamAPI_RunCallbacks_t)(void);
typedef BOOL(*SteamAPI_RestartAppIfNecessary_t)(void);
typedef void* (*SteamApps_t)(void);
typedef void* (*SteamGameServer_t)(void);
typedef void(*SteamGameServer_RunCallbacks_t)(void);
typedef void* (*SteamMasterServerUpdater_t)(void);
typedef BOOL(*SteamGameServer_Init_t)(void);
typedef void(*SteamGameServer_Shutdown_t)(void);
typedef void* (*SteamNetworking_t)(void);
typedef void* (*SteamUserStats_t)(void);
typedef void(*SteamAPI_RegisterCallback_t)(void);
typedef void* (*SteamUtils_t)(void);
typedef void* (*SteamUser_t)(void);
typedef void* (*SteamFriends_t)(void);

SteamAPI_Init_t o_SteamAPI_Init = nullptr;
SteamAPI_RegisterCallResult_t o_SteamAPI_RegisterCallResult = nullptr;
SteamAPI_Shutdown_t o_SteamAPI_Shutdown = nullptr;
SteamAPI_UnregisterCallback_t o_SteamAPI_UnregisterCallback = nullptr;
SteamAPI_UnregisterCallResult_t o_SteamAPI_UnregisterCallResult = nullptr;
SteamAPI_RunCallbacks_t o_SteamAPI_RunCallbacks = nullptr;
SteamAPI_RestartAppIfNecessary_t o_SteamAPI_RestartAppIfNecessary = nullptr;
SteamApps_t o_SteamApps = nullptr;
SteamGameServer_t o_SteamGameServer = nullptr;
SteamGameServer_RunCallbacks_t o_SteamGameServer_RunCallbacks = nullptr;
SteamMasterServerUpdater_t o_SteamMasterServerUpdater = nullptr;
SteamGameServer_Init_t o_SteamGameServer_Init = nullptr;
SteamGameServer_Shutdown_t o_SteamGameServer_Shutdown = nullptr;
SteamNetworking_t o_SteamNetworking = nullptr;
SteamUserStats_t o_SteamUserStats = nullptr;
SteamAPI_RegisterCallback_t o_SteamAPI_RegisterCallback = nullptr;
SteamUtils_t o_SteamUtils = nullptr;
SteamUser_t o_SteamUser = nullptr;
SteamFriends_t o_SteamFriends = nullptr;

// for loading function pointers
#define LOAD_FUNC(func) { \
    steam_functions[#func] = GetProcAddress(h_steam_api, #func); \
    if (!steam_functions[#func]) { \
        MessageBoxA(nullptr, "Failed to load " #func, "Error", MB_ICONERROR); \
    } else { \
        o_##func = reinterpret_cast<decltype(o_##func)>(steam_functions[#func]); \
    } \
}

// export the functions so bo1 knows to use our dll
#define EXPORT_FUNC(func, type) \
    extern "C" __declspec(dllexport) type func() { \
        if (o_##func) return o_##func(); \
        else { \
            MessageBoxA(nullptr, "Function " #func " not exported", "Error", MB_ICONERROR); \
            return (type)0; \
        } \
    }

// export the functions that bo1 imports from steam_api.dll
// we also make sure the function signatures match the originals
EXPORT_FUNC(SteamAPI_RegisterCallResult, void)
EXPORT_FUNC(SteamAPI_Shutdown, void)
EXPORT_FUNC(SteamAPI_UnregisterCallback, void)
EXPORT_FUNC(SteamAPI_UnregisterCallResult, void)
EXPORT_FUNC(SteamAPI_RunCallbacks, void)
EXPORT_FUNC(SteamAPI_RestartAppIfNecessary, BOOL)
EXPORT_FUNC(SteamApps, void*)
EXPORT_FUNC(SteamGameServer, void*)
EXPORT_FUNC(SteamGameServer_RunCallbacks, void)
EXPORT_FUNC(SteamMasterServerUpdater, void*)
EXPORT_FUNC(SteamGameServer_Init, BOOL)
EXPORT_FUNC(SteamGameServer_Shutdown, void)
EXPORT_FUNC(SteamNetworking, void*)
EXPORT_FUNC(SteamUserStats, void*)
EXPORT_FUNC(SteamAPI_RegisterCallback, void)
EXPORT_FUNC(SteamUtils, void*)
EXPORT_FUNC(SteamUser, void*)
EXPORT_FUNC(SteamFriends, void*)

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
        if (!o_SteamAPI_Init) {
            MessageBoxA(nullptr, "Failed to load SteamAPI_Init", "Error", MB_ICONERROR);
            return FALSE;
        }

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
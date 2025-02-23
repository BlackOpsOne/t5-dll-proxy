#include "pch.h"

#include "steam-hooks.hpp"

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	if (reason)
	{
		return steam_hooks::Initialize();
	}
	return FALSE;
}
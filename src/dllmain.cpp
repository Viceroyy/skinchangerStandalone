#include "App/App.h"

DWORD WINAPI MainThread(LPVOID lpParam)
{
#ifndef _WIN64
	while (!GetModuleHandle(L"mss32.dll"))
		Sleep(500); // In the wise words of spook953: we sleepin'
#else
	while (!GetModuleHandle(L"GameUI.dll")) //mss32.dll doesn't get loaded in 64-bit, this seems to be the next latest thing to get called before the game is fully loaded
		Sleep(500);
#endif

	g_App.Init();

	while (!g_App.ShouldStop())
	{
		Sleep(500);
	}

	g_App.Stop();

	Sleep(500);

	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpParam), EXIT_SUCCESS);

	return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		g_App.SetHandle(hModule);

		auto hThread = CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
		if (hThread)
			CloseHandle(hThread);
		else
			return FALSE;
	}

	return TRUE;
}


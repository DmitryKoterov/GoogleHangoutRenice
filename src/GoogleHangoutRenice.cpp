#include <windows.h>
#include <psapi.h>
#include <tchar.h>

static LPTSTR lpMutexName = _T("GooglHangoutRenice");

void SetPriorityAffinity(HANDLE hProcess)
{
	SetPriorityClass(hProcess, IDLE_PRIORITY_CLASS);
	DWORD_PTR processAffinityMask = 1;
	SetProcessAffinityMask(hProcess, processAffinityMask);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, lpMutexName);
	if (!hMutex) {
		hMutex = CreateMutex(0, 0, lpMutexName);
	} else {
		return 1;
	}

	while (1) {
		DWORD pids[1024], size;
		EnumProcesses(pids, sizeof(pids), &size);

		// Find GoogleTalkPlugin.
		BOOL foundGoogleTalkPlugin = FALSE;
		for (DWORD i = 0; i < size / sizeof(*pids); i++) {
			if (HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pids[i])) {
				HMODULE mods[256]; DWORD size;
				if (EnumProcessModules(hProcess, mods, sizeof(mods), &size)) {
					TCHAR szExeName[MAX_PATH + 1];
					if (GetModuleFileNameEx(hProcess, mods[0], szExeName, sizeof(szExeName))) {
						if (_tcsstr(szExeName, _T("googletalkplugin.exe"))) {
							SetPriorityAffinity(hProcess);
							foundGoogleTalkPlugin = TRUE;
						}
					}
				}
				CloseHandle(hProcess);
			}
		}

		// Find Firefox Plugin Container.
		// A bit copy+paste, but it's not a big problem, because this tool
		// is extremely simple - no need to build a spaceship here.
		if (foundGoogleTalkPlugin) {
			for (DWORD i = 0; i < size / sizeof(*pids); i++) {
				if (HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pids[i])) {
					HMODULE mods[256]; DWORD size;
					if (EnumProcessModules(hProcess, mods, sizeof(mods), &size)) {
						TCHAR szExeName[MAX_PATH + 1];
						if (GetModuleFileNameEx(hProcess, mods[0], szExeName, sizeof(szExeName))) {
							if (_tcsstr(szExeName, _T("plugin-container.exe"))) {
								SetPriorityAffinity(hProcess);
							}
						}
					}
					CloseHandle(hProcess);
				}
			}
		}

		// Wait a bit and retry.
		Sleep(1000 * 5);
	}

	return 0;
}

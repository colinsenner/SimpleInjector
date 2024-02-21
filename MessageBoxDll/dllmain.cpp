// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

#include "main.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  UNREFERENCED_PARAMETER(lpReserved);

  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
      DisableThreadLibraryCalls(hModule);

      auto hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Initialize, (LPVOID)hModule, 0, nullptr);

      if (hThread)
        WaitForSingleObject(hThread, 250);

      break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}

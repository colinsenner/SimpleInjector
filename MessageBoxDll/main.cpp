#include "main.h"

#include <Windows.h>

void UnloadDll(void* hModule)
{
  FreeLibraryAndExitThread((HMODULE)hModule, 0);
}

void Cleanup(void* hModule)
{
  UnloadDll(hModule);
}

DWORD Initialize(void* hModule)
{
  MessageBoxA(NULL, "Hello from DLL!", "DLL", MB_OK);

  Cleanup(hModule);

  return 0;
}

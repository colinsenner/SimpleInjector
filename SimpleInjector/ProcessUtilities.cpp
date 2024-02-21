#include "ProcessUtilities.h"

#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <vector>

std::vector<DWORD> PidFromProcessName(const wchar_t* processName)
{
  std::vector<DWORD> pids;

  HANDLE snapshot;
  PROCESSENTRY32 pe32;

  snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if (snapshot == INVALID_HANDLE_VALUE)
    return pids;

  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(snapshot, &pe32)) {
    CloseHandle(snapshot);
    return pids;
  }

  do {
    // wprintf(L"PROCESS NAME:  %s   PID: %d\n", pe32.szExeFile, pe32.th32ProcessID);

    // Process with a matching name (case-insensitive) was found
    if (_wcsicmp(pe32.szExeFile, processName) == 0)
      pids.push_back(pe32.th32ProcessID);

  } while (Process32Next(snapshot, &pe32));

  // Not found
  return pids;
}

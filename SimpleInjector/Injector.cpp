#include "Injector.h"

static FARPROC GetLoadLibraryAddress()
{
  HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");

  if (!hKernel32) {
    return nullptr;
  }

  return GetProcAddress(hKernel32, "LoadLibraryA");
}

static bool SetPrivilege(HANDLE hProcess, LPCTSTR privilegeName, bool bEnable)
{
  bool success = false;

  // Adjust token privileges to open system processes
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp = {0};

  if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
    LookupPrivilegeValue(NULL, privilegeName, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;

    if (bEnable)
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
      tkp.Privileges[0].Attributes = 0;

    AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
    CloseHandle(hToken);
    success = true;
  }

  return success;
}

bool Injector::Inject(DWORD pid, const char* pathToDll)
{
  auto _LoadLibraryA = GetLoadLibraryAddress();
  size_t remoteMemSize = strlen(pathToDll) + 1;

  if (!SetPrivilege(GetCurrentProcess(), SE_DEBUG_NAME, true))
    return false;

  // Open the process with all access
  HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  if (hProc == NULL)
    return false;

  // Allocate memory to hold the path to the Dll File in the process's memory
  LPVOID hRemoteMem = VirtualAllocEx(hProc, NULL, remoteMemSize, MEM_COMMIT, PAGE_READWRITE);

  if (hRemoteMem == NULL) {
    CloseHandle(hProc);
    return false;
  }

  // Write the path to the Dll File in the location just created
  SIZE_T numBytesWritten;
  WriteProcessMemory(hProc, hRemoteMem, pathToDll, remoteMemSize, &numBytesWritten);

  // Create a remote thread that starts begins at the LoadLibrary function and
  // is passed are memory pointer
  HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)_LoadLibraryA, hRemoteMem, 0, NULL);

  if (hRemoteThread == NULL) {
    CloseHandle(hProc);
    return false;
  }

  // Wait for the thread to finish
  WaitForSingleObject(hRemoteThread, INFINITE);
  DWORD hLibModule = 0;
  GetExitCodeThread(hRemoteThread, &hLibModule);

  // Free the memory created on the other process
  VirtualFreeEx(hProc, hRemoteMem, 0, MEM_RELEASE);

  // Release the handle to the other process
  CloseHandle(hProc);

  return true;
}

#pragma once
#include <Windows.h>

class Injector {
 public:
  static bool Inject(DWORD pid, const char* pathToDll);
};

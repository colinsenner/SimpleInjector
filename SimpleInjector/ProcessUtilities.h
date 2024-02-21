#pragma once
#include <Windows.h>
#include <vector>

std::vector<DWORD> PidFromProcessName(const wchar_t* processName);

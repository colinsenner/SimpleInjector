#include "StringUtilities.h"

#include <filesystem>

std::string FilenameFromPath(std::string path)
{
  auto filename = std::filesystem::path(path).filename();
  return filename.string();
}

std::wstring WideStringFromChar(const char* str)
{
  size_t size = strlen(str) + 1;
  wchar_t* wStr = new wchar_t[size];

  size_t outSize;
  mbstowcs_s(&outSize, wStr, size, str, size - 1);

  return std::wstring(wStr);
}

#include "FileUtilities.h"

#include <filesystem>

bool FileExists(std::string path)
{
  return std::filesystem::exists(path);
}

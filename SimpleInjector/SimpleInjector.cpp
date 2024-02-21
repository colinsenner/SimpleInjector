#include <stdio.h>
#include <stdlib.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "FileUtilities.h"
#include "Injector.h"
#include "ProcessUtilities.h"
#include "StringUtilities.h"

namespace fs = std::filesystem;

void DisplayUsage(char *argv[])
{
  std::string filename = FilenameFromPath(argv[0]);

  printf(
      "Usage: %s [process] [dll]\n"
      "\n"
      "Injects into the first process that matches the process name.\n"
      "    process - \"notepad.exe\"\n"
      "    dll     - \"C:\\Test\\inject.dll\"\n",
      filename.c_str());
}

int main(int argc, char *argv[])
{
  if (argc != 3) {
    DisplayUsage(argv);
    return 0;
  }

  std::string processName(argv[1]);
  std::string dllPath(argv[2]);

  std::string dllAbsolutePath = std::filesystem::absolute(dllPath).string();

  if (!FileExists(dllAbsolutePath)) {
    printf("Couldn't find dll \"%s\"\n", dllAbsolutePath.c_str());
    return 0;
  }

  auto wProcessName = WideStringFromChar(processName.c_str());
  auto pids = PidFromProcessName(wProcessName.c_str());

  if (pids.size() == 0) {
    printf("Couldn't find process with name \"%s\".\n", processName.c_str());
    system("pause");
    return 0;
  }
  else if (pids.size() > 1) {
    printf("Multiple processes with name \"%s\" found. Exiting.\n", processName.c_str());
    system("pause");
    return 0;
  }

  printf("Injecting \"%s\" into \"%s\" with pid %d.\n", dllAbsolutePath.c_str(), processName.c_str(), pids.front());

  if (!Injector::Inject(pids.front(), dllAbsolutePath.c_str())) {
    // Error
    return 1;
  }

  return 0;
}

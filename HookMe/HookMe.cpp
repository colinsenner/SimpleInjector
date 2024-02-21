#include <Windows.h>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
  int button = IDOK;

  while (button == IDOK) {
    button = MessageBoxA(NULL, "I'm original MessageBoxA", "Original", MB_OKCANCEL);
  }
}

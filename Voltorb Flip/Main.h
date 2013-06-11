#include <stdlib.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include "resource.h"
#include "Game.h"

RECT memo_rect = {199, 207, 248, 256};
RECT screen_rect = {0, 0, 256, 192*2};
RECT board_rect = {9, 201, 159, 351};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void handleLeftClick(int, int, HWND);
void handleRightClick(int, int, HWND);
bool in_rect(int, int, RECT);
RECT card_to_rect(int, int);
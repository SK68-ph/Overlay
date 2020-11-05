#ifndef HMAIN_H
#define HMAIN_H

#pragma once
#include "include.h"
#include <stdlib.h>
#include <ctime>
#include "hD3D9.h"

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")


extern int windowWidth;
extern int windowHeight; 
extern int overlayScale;
extern int overlayX_Pos;
extern int overlayY_Pos;
extern char lWindowName[256]; // Overlay Name
extern HWND hWnd; // Our programs handle
extern char tWindowName[256]; // Window name
extern HWND TargetWnd; // Target Exe Name
extern RECT WindowRect, ClientRect;
extern MSG Message;

LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif
#ifndef HDIRECTX_H
#define HDIRECTX_H
#pragma once
#include "hMain.h"
#pragma warning( disable : 4244 )
#pragma warning( disable : 4996 )

#include "include.h"
#include <algorithm>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

extern IDirect3D9Ex* dx_Object;
extern IDirect3DDevice9Ex* dx_Device;
extern D3DPRESENT_PARAMETERS dx_Params;
extern ID3DXLine* dx_Line;
extern ID3DXFont* dx_Font;


int D3D9Init(HWND hWnd);

int Render();

#endif
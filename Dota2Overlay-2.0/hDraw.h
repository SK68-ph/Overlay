#ifndef HDRAWFUNC_H
#define HDRAWFUNC_H

#include "hD3D9.h"

#pragma warning( disable : 4244 )
#pragma warning( disable : 4996 )

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


void GradientFunc(int x, int y, int w, int h, int r, int g, int b, int a);
void DrawCenterLine(float x, float y, int r, int g, int b, int a);
void DrawLine(float x, float y, float xx, float yy, int r, int g, int b, int a);
void DrawFilled(float x, float y, float w, float h, int r, int g, int b, int a);
void DrawBox(float x, float y, float width, float height, float px, int r, int g, int b, int a);
void DrawGUIBox(float x, float y, float w, float h, int r, int g, int b, int a, int rr, int gg, int bb, int aa);
void DrawHealthBar(float x, float y, float w, float h, int r, int g, int b, int a);
void DrawHealthBarBack(float x, float y, float w, float h, int a);

int DrawString(char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont);
int DrawShadowString(char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont);


#endif



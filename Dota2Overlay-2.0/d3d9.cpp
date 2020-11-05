
#include "hD3D9.h"
#include "Dota2MemReader.h"


IDirect3D9Ex* dx_Object = NULL;
IDirect3DDevice9Ex* dx_Device = NULL;
D3DPRESENT_PARAMETERS dx_Params;
ID3DXLine* dx_Line;
ID3DXFont* dx_Font = 0;

/*
We require to initialize the D3D drawing, so we require hWnd. Windows identifies each form or application by assigning it a handle or also known as hWnd.
*/
int D3D9Init(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &dx_Object)))
	{
		exit(1);
	}
	dx_Params.BackBufferFormat = D3DFMT_A8R8G8B8;

	dx_Params.BackBufferWidth = windowWidth;
	dx_Params.BackBufferHeight = windowHeight;
	dx_Params.EnableAutoDepthStencil = TRUE;
	dx_Params.AutoDepthStencilFormat = D3DFMT_D16;
	dx_Params.hDeviceWindow = hWnd;
	dx_Params.MultiSampleQuality = DEFAULT_QUALITY;
	dx_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dx_Params.Windowed = TRUE;

	if (FAILED(dx_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_Params, 0, &dx_Device)))
	{
		exit(1);
	}

	if (!dx_Line)
		D3DXCreateLine(dx_Device, &dx_Line);

	D3DXCreateFont(dx_Device, 50, 0, FW_LIGHT, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial", &dx_Font);
	return 0;

}


int Render()
{
	dx_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	dx_Device->BeginScene();

	if (TargetWnd == GetForegroundWindow())
	{
		int vbe = getVbe();
		if (vbe == 14)
		{
			DrawString((char*)"Visible", overlayX_Pos, overlayY_Pos, 255, 0, 0, dx_Font);
		}
		else if (vbe >= 6 && vbe <= 10)
		{
			DrawString((char*)"Not Visible", overlayX_Pos, overlayY_Pos, 0, 255, 0, dx_Font);
		}
		else
		{
			DrawString((char*)"Waiting", overlayX_Pos, overlayY_Pos, 255, 0, 0, dx_Font);
		}
		 // Put Main procedure here like ESP etc.
	}

	dx_Device->EndScene();
	dx_Device->PresentEx(0, 0, 0, 0, 0);

	return 0;
}
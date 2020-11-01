
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
	 // We get our Process Access and Module Bases
	/*
	We need to check to see if we can create an IDirect3D9Ex object and return an interface to it. Why is D3D_SDK_VERSION passed? Because we will need to ensure that the header files used in the compiled application match the version of the installed runtime DLLs. Why are we passing the object to dx_Object? Because we are creating an IDirect3D9Ex object, and we need to store it somewhere. If it fails, the app crashes (the DLL), and if it passes, it continues, simple huh?
	*/
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &dx_Object)))
	{
		exit(1);
	}

	/*
	We created the dx_Param earlier, it is a D3DPRESENT_PARAMETERS structure. It contains many variables you can modify but in this source we are only modifying these variables.

	BackBufferFormat (D3DFORMAT) is the buffer that is drawn off-screen and will be switched with the front buffer at the next frame. This is considered double buffering, which is what you need to do in GDI to ensure that it does not flicker. But GDI will still flicker because it is "slow" you could persay.

	D3DFMT_A8R8G8B8	(Value: 21) is an 32-bit ARGB pixel format with alpha, using 8 bits per channel.
	*/

	dx_Params.BackBufferFormat = D3DFMT_A8R8G8B8;

	dx_Params.BackBufferWidth = windowWidth;
	dx_Params.BackBufferHeight = windowHeight;
	dx_Params.EnableAutoDepthStencil = TRUE;
	dx_Params.AutoDepthStencilFormat = D3DFMT_D16;

	/*
	hDeviceWindow (HWND) is the form or application that determines the location and size of the back buffer on the screen.
	*/
	dx_Params.hDeviceWindow = hWnd;

	/*
	MultiSampleQuality (DWORD) is the quality level. Technically speaking DEFAULT_QUALITY is zero which also is kind of funny because zero is the lowest MultiSampleQuality. Why are we setting this? Well this is all GPU related, and microsoft is extremely vauge about this, so we will just leave this as zero.
	*/
	dx_Params.MultiSampleQuality = DEFAULT_QUALITY;

	/*
	SwapEffect (D3DSWAPEFFECT) is how the front and back buffer are to be swapped. When we disregard this, we can do multi sampling (above).
	*/
	dx_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;

	/*
	Windowed (BOOL) is basically asking you if the form or application is running windowed or fullscreen. True is windowed. False is fullscreen.
	*/
	dx_Params.Windowed = TRUE;


	/*
	We need to see if we can create a device to REPRESENT the display adapter.
	D3DADAPTER_DEFAULT (UNIT) is the always the primary display adapter.
	D3DDEVTYPE_HAL (D3DDEVTYPE) (value: 1) is hardware rasterization. Shading is done with software, hardware, or mixed transform and lighting.
	hWnd (HWND) is the form we will create the device in mind with. Something I noticed was that this can be null if we set the hDeviceWindow to a non-null value, which it is. So I changed it to a null.
	D3DCREATE_HARDWARE_VERTEXPROCESSING (DWORD) specifies hardware vertex processing.
	dx_Param (D3DPRESENT_PARAMTERS*) describe the presentation parameters for the device to be created.
	0 (D3DDISPLAYMODEEX*) is only used when the device is set to fullscreen, which it is not for this source, but it basically is the properties of a display mode (size, width, height, refresh rate, format, scanlineordering).
	dx_Device (IDirect3DDevice9Ex**) is the address of a pointer to the returned IDirect3DDevice9Ex, which represents the created device.
	*/
	if (FAILED(dx_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_Params, 0, &dx_Device)))
	{
		exit(1);
	}


	if (!dx_Line)
		D3DXCreateLine(dx_Device, &dx_Line);

	/*
	D3DXCreateFont creates a font object for a device and font.

	D3DXCreateFont(device, h, w, weight, miplevels, italic, charset, OutputPrecision, quality, pitchandfamily, pfaceanme, *ppfont)

	dx_Device (LPDIRECT3DDEVICE9) is the device we will be creating a font for.
	24 (INT) is the height of the characters in logical units.
	0 (UINT) is the width of the characters in logical units.
	FW_REGULAR (UNIT) is the typeface weight.
	0 (MipLevels) is the number of mipmap levels. MipMaps (not miplevels) are pre-calculated, optimized collections of images that accompany a main texture, intended to increase rendering speed and reduce aliasing artifacts
	false (BOOL) is if the italic font is true or not. In this case it is false.
	DEFAULT_CHARSET ( DWORD) is the character set of the font.
	OUT_CHARACTER_PRECIS (DWORD) specifies how Windows should attempt to match the desired font sizes and characteristics with actual fonts. In this case we are not using this feature.
	ANTIALIASED_QUALITY (DWORD) specifies how Windows should match the desired font with a real font. In this case we are always antialiasing if the font supports it and the size of the font is not too small or too large.
	DEFAULT_PITCH (DWORD) is the pitch and family index.
	Verdana (LPCTSTR) is the string containing the typeface name (font style).
	dx_Font (LPD3DXFONT*) returns a pointer to an ID3DXFont interface, representing the created font object.
	*/
	D3DXCreateFont(dx_Device, 50, 0, FW_LIGHT, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial", &dx_Font);
	return 0;

}

int getPercent(float percent, float value)
{
	return (percent / 100) * value;
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
			DrawString((char*)"Visible", getPercent(overlaySize, windowWidth), getPercent(4, windowHeight), 255, 0, 0, dx_Font);
		}
		else if (vbe >= 6 && vbe <= 10)
		{
			DrawString((char*)"Not Visible", getPercent(overlaySize, windowWidth), getPercent(4, windowHeight), 0, 255, 0, dx_Font);
		}
		else
		{
			DrawString((char*)"Waiting", getPercent(overlaySize, windowWidth), getPercent(4, windowHeight), 255, 0, 0, dx_Font);
		}
		 // Put Main procedure here like ESP etc.
	}

	dx_Device->EndScene();
	dx_Device->PresentEx(0, 0, 0, 0, 0);

	return 0;
}
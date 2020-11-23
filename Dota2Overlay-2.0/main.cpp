#include "hMain.h"
#include "Dota2MemReader.h"

HWND hWnd, TargetWnd;
MSG Message;
RECT WindowRect, ClientRect;
int windowWidth, windowHeight, overlayScale, overlayX_Pos, overlayY_Pos;
int clientWidth = 1920, clientHeight = 1080;
int borderWidth, borderHeight; // Grenz Größen
int savedX_Pos,savedY_Pos,savedFont_Size;

char lWindowName[256] = "GG";
char tWindowName[256] = "Dota 2"; // put Game window name here

const MARGINS pMargin = { 0,0, clientWidth, clientHeight };


LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{

	case WM_PAINT:


		if (TargetWnd == GetForegroundWindow())
		{
			dx_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
			dx_Device->BeginScene();
			Render();
			dx_Device->EndScene();
			dx_Device->PresentEx(0, 0, 0, 0, 0);
		}
		else
		{
			dx_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
			dx_Device->BeginScene();

			dx_Device->EndScene();
			dx_Device->PresentEx(0, 0, 0, 0, 0);
		}

		break;
	case WM_CREATE:
		return DwmExtendFrameIntoClientArea(hWnd, &pMargin); 
		break;
	case WM_DESTROY:
		PostQuitMessage(0); 
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam); 
		break;
	}
}

int getPercent(float percent, float value)
{
	return (percent / 100) * value;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXA OverlayWnd; // contains window class information
	OverlayWnd.cbSize = sizeof(WNDCLASSEXA); // size of struct, basically checking for version or check
	OverlayWnd.style = CS_HREDRAW | CS_VREDRAW;  // Style, redraw method type
	OverlayWnd.lpfnWndProc = Proc; // Pointer to the window procedure
	OverlayWnd.cbClsExtra = 0; // window class struct extra bytes
	OverlayWnd.cbWndExtra = 0; // window instance extra bytes
	OverlayWnd.hInstance = hInstance; // handle to the instance that contains the window procedure for the class
	OverlayWnd.hIcon = LoadIcon(NULL, IDI_APPLICATION); // basic window icon set
	OverlayWnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // basic window icon set
	OverlayWnd.hCursor = LoadCursor(NULL, IDC_ARROW); // basic window cursor icon set
	OverlayWnd.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); // handle to the class background brush
	OverlayWnd.lpszMenuName = lWindowName;
	OverlayWnd.lpszClassName = lWindowName;

	if (!RegisterClassExA(&OverlayWnd))
		exit(1);

	TargetWnd = FindWindowA(0, tWindowName);


	if (TargetWnd)
	{
		overlayScale = 45;
		GetWindowRect(TargetWnd, &WindowRect);
		windowWidth = WindowRect.right - WindowRect.left;
		windowHeight = WindowRect.bottom - WindowRect.top;
		clientHeight = windowHeight;
		clientWidth = windowWidth;
		hWnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, lWindowName, lWindowName, WS_POPUP, 1, 1, windowWidth, windowHeight, 0, 0, 0, 0);
		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
		SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
		ShowWindow(hWnd, SW_SHOW);
	}

	if (initHack() == -1)
	{
		MessageBox(NULL, L"Overlay failed to initialize...", L"Error", MB_ICONERROR);
		exit(1);
	}

	D3D9Init(hWnd);

	//While Delete key is not pressed, we will be enable our hack.
	while (!GetAsyncKeyState(VK_DELETE))
	{
		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		TargetWnd = FindWindowA(0, tWindowName);

		if (!TargetWnd)
			exit(1);

		GetWindowRect(TargetWnd, &WindowRect);
		GetClientRect(TargetWnd, &ClientRect);

		windowWidth = WindowRect.right - WindowRect.left;
		windowHeight = WindowRect.bottom - WindowRect.top;
		DWORD dwStyle = GetWindowLong(TargetWnd, GWL_STYLE);

		if (dwStyle & WS_BORDER)
		{

			windowHeight = WindowRect.bottom - WindowRect.top;
			windowWidth = WindowRect.right - WindowRect.left;
			clientHeight = ClientRect.bottom - ClientRect.top;
			clientWidth = ClientRect.right - ClientRect.left;
			borderHeight = (windowHeight - ClientRect.bottom);
			borderWidth = (windowWidth - ClientRect.right) / 2; //only want one side
			borderHeight -= borderWidth; //remove bottom from width, bottom is the same size as either side so we subtract that

			WindowRect.left += borderWidth;
			WindowRect.top += borderHeight;
			_RPT1(0, "Window He = %u", windowHeight);

		}

		overlayX_Pos = getPercent(overlayScale, clientWidth);
		overlayY_Pos = getPercent(4, clientHeight);
		// if insert is pressed we move our TextOverlay to current cursor position
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			POINT p;					
			if (GetCursorPos(&p))
			{
				if (ScreenToClient(hWnd, &p))
				{
					overlayX_Pos = p.x;
					overlayY_Pos = p.y;
					setConfig();
				}
			}
			
		}
		// if PageUp is pressed we increase font size
		if (GetAsyncKeyState(VK_PRIOR) & 1)
		{
			setFontSize(fontSize += 2);
			setConfig();
		}
		// if PageDown is pressed we deccrease font size
		else if (GetAsyncKeyState(VK_NEXT) & 1)
		{
			setFontSize(fontSize -= 2);
			setConfig();
		}
		MoveWindow(hWnd, WindowRect.left, WindowRect.top, clientWidth, clientHeight, true);

	} // End of Loop
	setConfig();

	/*
	Lets exit immediately...
	*/
	close();
	exit(0);

}